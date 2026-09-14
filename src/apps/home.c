#include "home.h"
#include "files.h"
#include "settings.h"
#include "network.h"
#include "gui/window_manager.h"
#include "gui/widgets.h"
#include "gui/theme.h"
#include "hal/display.h"
#include "hal/sound.h"
#include "hal/input.h"
#include "hal/wifi.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>

// ─── App registry ────────────────────────────────────────────────────────────

typedef void (*launch_fn)(void);

typedef struct {
    const char* name;    // display label (shown under card)
    const char* abbr;    // 2-char text drawn on the card
    uint16_t    color;   // card background color
    launch_fn   launch;
} AppEntry;

#define RGB(r,g,b) ((uint16_t)(((r&0xF8u)<<8)|((g&0xFCu)<<3)|((b)>>3)))

static AppEntry apps[] = {
    { "Files",    "FL", RGB(230, 110,  20), app_files_launch    },
    { "Network",  "NW", RGB( 20, 100, 220), app_network_launch  },
    { "Settings", "ST", RGB( 50,  60,  75), app_settings_launch },
    { "Shell",    ">_", RGB( 20, 140,  80), NULL                },
};
#define NUM_APPS ((int)(sizeof(apps)/sizeof(apps[0])))

// ─── State ───────────────────────────────────────────────────────────────────

static int  sel         = 0;    // selected app index
static bool needs_draw  = true; // dirty flag
static bool glow_on     = true; // glow pulse state
static uint32_t last_glow_ms = 0;

// ─── Layout helpers ──────────────────────────────────────────────────────────
//
// Show up to 4 cards at a time, centered on screen.
// If NUM_APPS <= 4 all cards fit; otherwise we'd scroll (future).
//
// For N cards: total_w = N*CARD_W + (N-1)*CARD_GAP
// start_x = (320 - total_w) / 2

static int  n_visible(void)  { return NUM_APPS < 4 ? NUM_APPS : 4; }
static int16_t row_start_x(void) {
    int n = n_visible();
    int tw = n * CARD_W + (n - 1) * CARD_GAP;
    return (320 - tw) / 2;
}
static int16_t card_x(int idx) {
    return row_start_x() + idx * (CARD_W + CARD_GAP);
}

// ─── Draw ────────────────────────────────────────────────────────────────────

static void draw_background(void) {
    display_fill(THEME_BG);
}

static void draw_cards(void) {
    int n = n_visible();
    for (int i = 0; i < n; i++) {
        bool selected = (i == sel);
        // Clear area around card (erase old glow)
        display_draw_rect(card_x(i) - 5, CARD_Y - 5,
                          CARD_W + 10, CARD_H + 10, THEME_BG);
        widget_card(card_x(i), CARD_Y, apps[i].color, apps[i].abbr, selected);

        // Label below card
        uint16_t label_col = selected ? THEME_TEXT_SEL : THEME_TEXT_DIM;
        widget_text_center(card_x(i), LABEL_Y, CARD_W,
                           apps[i].name, label_col, THEME_BG, LABEL_SCALE);
    }
}

static void draw_dots(void) {
    widget_dots(160, DOTS_Y, NUM_APPS, sel);
}

static void draw_status(void) {
    // TODO: read RTC for real time. Static placeholder for now.
    widget_status_bar("12:00", wifi_is_connected());
}

static void draw_actions(void) {
    widget_action_bar("(A) Open", "(<>) Move");
}

void app_home_draw(void) {
    if (!needs_draw) return;
    needs_draw = false;

    draw_background();
    draw_status();
    draw_cards();
    draw_dots();
    draw_actions();
}

// ─── Navigation ──────────────────────────────────────────────────────────────

static void nav_left(void) {
    if (sel > 0) { sel--; needs_draw = true; sound_play_click(); }
}
static void nav_right(void) {
    if (sel < NUM_APPS - 1) { sel++; needs_draw = true; sound_play_click(); }
}
static void nav_open(void) {
    if (apps[sel].launch) {
        sound_play_tone(880, 40);
        apps[sel].launch();
    }
}

// ─── Touch hit-test ──────────────────────────────────────────────────────────
// Called by window_manager when the touch driver fires INPUT_SELECT.
// x,y are calibrated display coordinates.

void app_home_touch(int16_t x, int16_t y) {
    int n = n_visible();
    for (int i = 0; i < n; i++) {
        int16_t cx = card_x(i);
        // Hit the card body or label area
        if (x >= cx && x < cx + CARD_W &&
            y >= CARD_Y && y < LABEL_Y + LABEL_SCALE * 7) {
            if (i == sel) {
                nav_open();   // tap selected card again → open
            } else {
                sel = i;
                needs_draw = true;
                sound_play_click();
            }
            return;
        }
    }
}

// ─── Tick (called each scheduler loop) ───────────────────────────────────────
// Handles input polling and glow animation.

void app_home_tick(void) {
    // Input
    InputEvent ev = input_poll();
    switch (ev) {
        case INPUT_LEFT:   nav_left();  break;
        case INPUT_RIGHT:  nav_right(); break;
        case INPUT_SELECT: nav_open();  break;
        default: break;
    }

    // Glow pulse every 600 ms: redraw only the selected card
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_glow_ms > 600) {
        last_glow_ms = now;
        glow_on      = !glow_on;

        // Redraw only the selected card (avoid full-screen repaint)
        int16_t cx = card_x(sel);
        display_draw_rect(cx - 5, CARD_Y - 5, CARD_W + 10, CARD_H + 10, THEME_BG);
        widget_card(cx, CARD_Y, apps[sel].color, apps[sel].abbr, glow_on);
        // Re-draw label so it isn't erased
        uint16_t lc = THEME_TEXT_SEL;
        widget_text_center(cx, LABEL_Y, CARD_W, apps[sel].name, lc, THEME_BG, LABEL_SCALE);
    }

    // Full draw if flagged
    if (needs_draw) app_home_draw();
}

// ─── Window descriptor ───────────────────────────────────────────────────────

static Window home_win = {
    .title    = "Home",
    .on_draw  = app_home_draw,
    .on_touch = app_home_touch,
    .active   = false,
};

void app_home_launch(void) {
    sel        = 0;
    needs_draw = true;
    wm_push(&home_win);
    sound_play_boot();
}
