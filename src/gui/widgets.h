#pragma once
#include <stdint.h>

// Draw a Switch-style app card
void widget_card(int16_t x, int16_t y, uint16_t bg_color, const char* label);

// Draw the top status bar
void widget_status_bar(const char* time_str, bool wifi);

// Draw a rounded rectangle
void widget_roundrect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

// Draw a label centered in a region
void widget_label_center(int16_t x, int16_t y, int16_t w, const char* text, uint16_t color);

