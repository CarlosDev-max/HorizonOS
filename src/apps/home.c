#include "home.h"
#include "gui/window_manager.h"
#include "gui/widgets.h"
#include "gui/theme.h"
#include "hal/display.h"
#include "hal/wifi.h"

// App cards shown on the home screen
typedef struct { const char* name; uint16_t color; } AppCard;

static AppCard cards[] = {
    { "Files",    0x2D6B },
    { "Network",  0x0453 },
    { "Settings", 0xF800 },
};
#define CARD_COUNT 3

static Window home_win = {
    .title    = "Home",
    .on_draw  = app_home_draw,
    .on_touch = app_home_touch,
};

void app_home_launch(void) {
    wm_push(&home_win);
}

void app_home_draw(void) {
    // Background
    display_fill(THEME_BG);

    // Status bar
    widget_status_bar("12:00", wifi_is_connected());

    // App cards centered row
    int16_t total_w = CARD_COUNT * CARD_W + (CARD_COUNT - 1) * CARD_GAP;
    int16_t start_x = (DISPLAY_W - total_w) / 2;
    int16_t card_y  = (DISPLAY_H - CARD_H - 16) / 2 + STATUS_BAR_H;

    for (int i = 0; i < CARD_COUNT; i++) {
        int16_t cx = start_x + i * (CARD_W + CARD_GAP);
        widget_card(cx, card_y, cards[i].color, cards[i].name);
    }
}

void app_home_touch(int16_t x, int16_t y) {
    // TODO: detect which card was tapped and launch it
}

