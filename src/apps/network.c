#include "network.h"
#include "gui/window_manager.h"
#include "gui/theme.h"
#include "hal/display.h"
#include "hal/wifi.h"
#include "pico/stdlib.h"

static void net_draw(void) {
    display_fill(THEME_BG);
    display_draw_text_scaled(8, 10, "Network", THEME_ACCENT, THEME_BG, 2);
    const char* ip = wifi_ip();
    display_draw_text(8, 40, wifi_is_connected() ? ip : "Not connected", THEME_TEXT, THEME_BG);
    display_draw_text(8, 55, "(tap to go back)", THEME_TEXT_DIM, THEME_BG);
}

static void net_touch(int16_t x, int16_t y) {
    (void)x; (void)y;
    wm_pop();
}

static Window net_win = {
    .title    = "Network",
    .on_draw  = net_draw,
    .on_touch = net_touch,
    .active   = false,
};

void app_network_launch(void) {
    wm_push(&net_win);
}
