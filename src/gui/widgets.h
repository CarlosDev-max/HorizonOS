#pragma once
#include <stdint.h>
#include <stdbool.h>

// Filled rounded rectangle (Bresenham corners, no float)
void widget_roundrect(int16_t x, int16_t y, int16_t w, int16_t h,
                      int16_t r, uint16_t color);

// App card (72x72 rounded rect with 2-char abbreviation centered)
// selected=true draws a cyan glow border around it
void widget_card(int16_t x, int16_t y, uint16_t bg,
                 const char* abbr, bool selected);

// Top status bar — hostname left, wifi+time right
void widget_status_bar(const char* time_str, bool wifi_ok);

// Bottom action bar — Switch-style button hints
void widget_action_bar(const char* left_hint, const char* right_hint);

// Pagination dots row
void widget_dots(int16_t cx, int16_t y, uint8_t count, uint8_t active);

// Center a text string horizontally within a region
void widget_text_center(int16_t x, int16_t y, int16_t w,
                         const char* text, uint16_t fg, uint16_t bg, uint8_t scale);
