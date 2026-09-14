#pragma once
#include <stdint.h>

#define DISPLAY_W 320
#define DISPLAY_H 240

#define DISPLAY_SPI  spi0
#define DISPLAY_SCK  18
#define DISPLAY_MOSI 19
#define DISPLAY_MISO 16
#define DISPLAY_CS   17
#define DISPLAY_DC   20
#define DISPLAY_RST  21
#define DISPLAY_BL   22

void display_init(void);
void display_fill(uint16_t color);
void display_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void display_draw_pixel(int16_t x, int16_t y, uint16_t color);
void display_draw_text(int16_t x, int16_t y, const char* text, uint16_t fg, uint16_t bg);
void display_draw_text_scaled(int16_t x, int16_t y, const char* text,
                               uint16_t fg, uint16_t bg, uint8_t scale);
void display_draw_rect_border(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint8_t thickness, uint16_t color);
void display_flush(void);

// RGB565 color helpers
#define RGB(r,g,b) ((uint16_t)(((r&0xF8u)<<8)|((g&0xFCu)<<3)|((b)>>3)))
#define COLOR_BLACK   RGB(0,   0,   0)
#define COLOR_WHITE   RGB(255, 255, 255)
#define COLOR_NAVY    RGB(24,  24,  48)
#define COLOR_ACCENT  RGB(100, 210, 245)
#define COLOR_ACCENT2 RGB(255, 100,  30)
