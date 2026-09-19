#include "settings.h"
#include "gui/window_manager.h"
#include "gui/theme.h"
#include "hal/display.h"
#include "pico/stdlib.h"

static void settings_draw(void) {
    display_fill(THEME_BG);
    display_draw_text_scaled(8, 10, "Settings", THEME_ACCENT, THEME_BG, 2);
    display_draw_text(8, 40, "Version: 0.1-dev", THEME_TEXT, THEME_BG);
    display_draw_text(8, 55, "Board:   Pico 2W", THEME_TEXT, THEME_BG);
    display_draw_text(8, 70, "(tap to go back)", THEME_TEXT_DIM, THEME_BG);
}

static void settings_touch(int16_t x, int16_t y) {
    (void)x; (void)y;
    wm_pop();
}

static Window settings_win = {
    .title    = "Settings",
    .on_draw  = settings_draw,
    .on_touch = settings_touch,
    .active   = false,
};

void app_settings_launch(void) {
    wm_push(&settings_win);
}
