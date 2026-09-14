#include "widgets.h"
#include "theme.h"
#include "hal/display.h"
#include <string.h>

// ─── Rounded rectangle (integer-only Bresenham corners) ─────────────────────

void widget_roundrect(int16_t x, int16_t y, int16_t w, int16_t h,
                      int16_t r, uint16_t color) {
    if (r <= 0 || w < 2*r || h < 2*r) {
        display_draw_rect(x, y, w, h, color);
        return;
    }
    // Three-rect cross (body)
    display_draw_rect(x + r,     y,         w - 2*r, h,         color);
    display_draw_rect(x,         y + r,     r,       h - 2*r,   color);
    display_draw_rect(x + w - r, y + r,     r,       h - 2*r,   color);

    // Fill each corner with rows calculated via integer sqrt
    for (int dy = 0; dy < r; dy++) {
        int d  = r - 1 - dy;
        int sq = r*r - d*d;
        int dx = 1;
        while ((dx+1)*(dx+1) <= sq) dx++;          // integer sqrt

        // top-left  corner: fill from (x + r - dx) rightward by dx
        display_draw_rect(x + r - dx,     y + dy,         dx, 1, color);
        // top-right corner: fill from (x + w - r) rightward by dx
        display_draw_rect(x + w - r,      y + dy,         dx, 1, color);
        // bottom-left
        display_draw_rect(x + r - dx,     y + h - 1 - dy, dx, 1, color);
        // bottom-right
        display_draw_rect(x + w - r,      y + h - 1 - dy, dx, 1, color);
    }
}

// ─── Text center helper ──────────────────────────────────────────────────────

// char width in pixels at given scale (font col + 1 gap, times scale)
#define CHAR_PX(scale) ((5 * (scale)) + (scale))

void widget_text_center(int16_t x, int16_t y, int16_t w,
                         const char* text, uint16_t fg, uint16_t bg, uint8_t scale) {
    int len     = (int)strlen(text);
    int text_px = len * CHAR_PX(scale) - scale; // subtract trailing gap
    int16_t tx  = x + (w - text_px) / 2;
    if (tx < x) tx = x;
    display_draw_text_scaled(tx, y, text, fg, bg, scale);
}

// ─── App card ────────────────────────────────────────────────────────────────

void widget_card(int16_t x, int16_t y, uint16_t bg,
                 const char* abbr, bool selected) {
    if (selected) {
        // Outer glow ring (2 px, accent color)
        display_draw_rect_border(x - 3, y - 3, CARD_W + 6, CARD_H + 6, 2, THEME_ACCENT);
        // Thin dark separator inside glow
        display_draw_rect_border(x - 1, y - 1, CARD_W + 2, CARD_H + 2, 1, THEME_BG);
    }
    // Card body
    widget_roundrect(x, y, CARD_W, CARD_H, CARD_R, bg);

    // 2-char abbreviation centered on the card (scale=3 → 15x21 px per char)
    // Each char at scale 3: width=(5*3)+3=18px, height=7*3=21px
    // 2 chars: 18+18=36px wide (approx), centered in CARD_W=72 → offset=(72-36)/2=18
    int16_t abbr_x = x + (CARD_W - (int16_t)(strlen(abbr) * 18)) / 2;
    int16_t abbr_y = y + (CARD_H - 21) / 2;
    // Draw on card with card color as bg so it blends
    uint16_t text_col = RGB(255,255,255);
    display_draw_text_scaled(abbr_x, abbr_y, abbr, text_col, bg, 3);
}

// ─── Status bar ──────────────────────────────────────────────────────────────

void widget_status_bar(const char* time_str, bool wifi_ok) {
    display_draw_rect(0, 0, 320, STATUS_BAR_H, THEME_BG_DARKER);

    // Separator line at bottom of bar
    display_draw_rect(0, STATUS_BAR_H - 1, 320, 1, THEME_ACCENT);

    // Left: OS name
    display_draw_text_scaled(8, 4, "HorizonOS", THEME_ACCENT, THEME_BG_DARKER, 1);

    // Right: WiFi indicator
    const char* wifi_str = wifi_ok ? "~WiFi" : " ---";
    uint16_t    wifi_col = wifi_ok ? THEME_ACCENT : THEME_TEXT_DIM;
    int16_t     wx       = 320 - (int16_t)(strlen(wifi_str) * 6) - 60;
    display_draw_text(wx, 7, wifi_str, wifi_col, THEME_BG_DARKER);

    // Right: Time
    int16_t tx = 320 - (int16_t)(strlen(time_str) * 6) - 6;
    display_draw_text(tx, 7, time_str, THEME_TEXT, THEME_BG_DARKER);
}

// ─── Action bar (Switch-style button hints) ──────────────────────────────────
// Draws a bar at the very bottom: [hint_left]         [hint_right]
// The ● / ◄► symbols are approximated with ASCII since we have a 5x7 font.

void widget_action_bar(const char* left_hint, const char* right_hint) {
    int16_t y = 240 - ACTION_BAR_H;
    display_draw_rect(0, y, 320, ACTION_BAR_H, THEME_BG_DARKER);
    display_draw_rect(0, y, 320, 1, THEME_ACCENT); // top border line

    // Left hint: e.g. "(A) Open"
    display_draw_text(8, y + 5, left_hint, THEME_TEXT, THEME_BG_DARKER);

    // Right hint: e.g. "(</>) Scroll"
    int16_t rx = 320 - (int16_t)(strlen(right_hint) * 6) - 8;
    display_draw_text(rx, y + 5, right_hint, THEME_TEXT_DIM, THEME_BG_DARKER);
}

// ─── Pagination dots ─────────────────────────────────────────────────────────
// Small filled circles (approximated as 4x4 squares for simplicity on 5x7 font display)

void widget_dots(int16_t cx, int16_t y, uint8_t count, uint8_t active) {
    const int DOT = 5;   // dot size
    const int GAP = 4;   // gap between dots
    int total = count * DOT + (count - 1) * GAP;
    int16_t x = cx - total / 2;

    for (uint8_t i = 0; i < count; i++) {
        uint16_t col = (i == active) ? THEME_ACCENT : THEME_TEXT_DIM;
        // Draw filled square as dot
        display_draw_rect(x, y, DOT, DOT, col);
        x += DOT + GAP;
    }
}
