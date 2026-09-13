#include "widgets.h"
#include "theme.h"
#include "hal/display.h"

void widget_card(int16_t x, int16_t y, uint16_t bg_color, const char* label) {
    widget_roundrect(x, y, CARD_W, CARD_H, CARD_RADIUS, bg_color);
    widget_label_center(x, y + CARD_H + 4, CARD_W, label, THEME_TEXT);
}

void widget_status_bar(const char* time_str, bool wifi) {
    display_draw_rect(0, 0, DISPLAY_W, STATUS_BAR_H, THEME_BAR_BG);
    display_draw_text(8, 4, "HorizonOS", THEME_ACCENT, THEME_BAR_BG);
    display_draw_text(DISPLAY_W - 60, 4, time_str, THEME_TEXT, THEME_BAR_BG);
    display_draw_text(DISPLAY_W - 16, 4, wifi ? "W" : "-", wifi ? THEME_ACCENT : THEME_TEXT_MUTED, THEME_BAR_BG);
}

void widget_roundrect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    // TODO: proper rounded rect with corner arcs
    display_draw_rect(x, y, w, h, color);
}

void widget_label_center(int16_t x, int16_t y, int16_t w, const char* text, uint16_t color) {
    // TODO: measure text width and center it
    display_draw_text(x + 4, y, text, color, THEME_BG);
}

