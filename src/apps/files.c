#include "files.h"
#include "gui/window_manager.h"
#include "gui/theme.h"
#include "hal/display.h"
#include "pico/stdlib.h"

static void files_draw(void) {
    display_fill(THEME_BG);
    display_draw_text_scaled(8, 10, "Files", THEME_ACCENT, THEME_BG, 2);
    display_draw_text(8, 40, "(coming soon)", THEME_TEXT_DIM, THEME_BG);
}

static void files_touch(int16_t x, int16_t y) {
    (void)x; (void)y;
    wm_pop();
}

static Window files_win = {
    .title    = "Files",
    .on_draw  = files_draw,
    .on_touch = files_touch,
    .active   = false,
};

void app_files_launch(void) {
    wm_push(&files_win);
}
