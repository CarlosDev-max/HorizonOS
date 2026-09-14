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
    const char* name;
    const char* abbr;
    uint16_t    color;
    launch_fn   launch;
} AppEntry;

static AppEntry apps[] = {
    { "Files",    "FL", APP_COLOR_FILES, app_files_launch    },
    { "Network",  "NW", APP_COLOR_NET,   app_network_launch  },
    { "Settings", "ST", APP_COLOR_SETT,  app_settings_launch },
    { "Shell",    ">_", APP_COLOR_SHELL, NULL                },
};
#define NUM_APPS ((int)(sizeof(apps)/sizeof(apps[0])))

// ─── State ───────────────────────────────────────────────────────────────────

static int     sel          = 0;
static bool    needs_draw   = true;
static bool    glow_on      = true;
static uint32_t last_glow_ms = 0;

// ─── Layout ──────────────────────────────────────────────────────────────────

static int n_visible(void)       { return NUM_APPS < 4 ? NUM_APPS : 4; }
static int16_t row_start_x(void) {
    int n  = n_visible();
    int tw = n * CARD_W + (n - 1) * CARD_GAP;
    return (int16_t)((320 - tw) / 2);
}
static int16_t card_x(int idx) {
    return row_start_x() + (int16_t)(idx * (CARD_W + CARD_GAP));
}

// ─── Draw ────────────────────────────────────────────────────────────────────

static void draw_cards(void) {
    int n = n_visible();
    for (int i = 0; i < n; i++) {
        bool selected = (i == sel);
        display_draw_rect(card_x(i) - 5, CARD_Y - 5,
                          CARD_W + 10, CARD_H + 10, THEME_BG);
        widget_card(card_x(i), CARD_Y, apps[i].color, apps[i].abbr, selected);
        uint16_t lc = selected ? THEME_TEXT_SEL : THEME_TEXT_DIM;
        widget_text_center(card_x(i), LABEL_Y, CARD_W,
                           apps[i].name, lc, THEME_BG, LABEL_SCALE);
    }
}

void app_home_draw(void) {
    if (!needs_draw) return;
    needs_draw = false;
    display_fill(THEME_BG);
    widget_status_bar("12:00", wifi_is_connected());
    draw_cards();
    widget_dots(160, DOTS_Y, NUM_APPS, (uint8_t)sel);
    widget_action_bar("(A) Open", "(<>) Move");
}

// ─── Navigation ──────────────────────────────────────────────────────────────

static void nav_left(void)  {
    if (sel > 0) { sel--; needs_draw = true; sound_play_click(); }
}
static void nav_right(void) {
    if (sel < NUM_APPS-1) { sel++; needs_draw = true; sound_play_click(); }
}
static void nav_open(void)  {
    if (apps[sel].launch) {
        sound_play_tone(880, 40);
        apps[sel].launch();
    }
}

void app_home_touch(int16_t x, int16_t y) {
    int n = n_visible();
    for (int i = 0; i < n; i++) {
        int16_t cx = card_x(i);
        if (x >= cx && x < cx + CARD_W &&
            y >= CARD_Y && y < LABEL_Y + LABEL_SCALE * 7) {
            if (i == sel) nav_open();
            else { sel = i; needs_draw = true; sound_play_click(); }
            return;
        }
    }
}

// ─── Tick ────────────────────────────────────────────────────────────────────

void app_home_tick(void) {
    InputEvent ev = input_poll();
    switch (ev) {
        case INPUT_LEFT:   nav_left();  break;
        case INPUT_RIGHT:  nav_right(); break;
        case INPUT_SELECT: nav_open();  break;
        default: break;
    }

    // Glow pulse every 600 ms — only redraws selected card
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_glow_ms > 600) {
        last_glow_ms = now;
        glow_on      = !glow_on;
        int16_t cx   = card_x(sel);
        display_draw_rect(cx-5, CARD_Y-5, CARD_W+10, CARD_H+10, THEME_BG);
        widget_card(cx, CARD_Y, apps[sel].color, apps[sel].abbr, glow_on);
        widget_text_center(cx, LABEL_Y, CARD_W,
                           apps[sel].name, THEME_TEXT_SEL, THEME_BG, LABEL_SCALE);
    }

    if (needs_draw) app_home_draw();
}

// ─── Window ──────────────────────────────────────────────────────────────────

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
