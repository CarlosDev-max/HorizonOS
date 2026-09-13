#include "display.h"
#include "font5x7.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"
#include "pico/stdlib.h"
#include <string.h>

// ─── Low-level SPI helpers ───────────────────────────────────────────────────

static inline void display_cmd(uint8_t cmd) {
    gpio_put(DISPLAY_DC, 0);
    gpio_put(DISPLAY_CS, 0);
    spi_write_blocking(DISPLAY_SPI, &cmd, 1);
    gpio_put(DISPLAY_CS, 1);
}

static inline void display_data(const uint8_t* data, size_t len) {
    gpio_put(DISPLAY_DC, 1);
    gpio_put(DISPLAY_CS, 0);
    spi_write_blocking(DISPLAY_SPI, data, len);
    gpio_put(DISPLAY_CS, 1);
}

static inline void display_data1(uint8_t b) {
    display_data(&b, 1);
}

// Set pixel address window — all subsequent RAMWR pixels fill this region
static void display_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t d[4];
    display_cmd(0x2A);                                    // CASET
    d[0]=x0>>8; d[1]=x0&0xFF; d[2]=x1>>8; d[3]=x1&0xFF;
    display_data(d, 4);
    display_cmd(0x2B);                                    // PASET
    d[0]=y0>>8; d[1]=y0&0xFF; d[2]=y1>>8; d[3]=y1&0xFF;
    display_data(d, 4);
    display_cmd(0x2C);                                    // RAMWR
}

// Write 'count' pixels all the same RGB565 color
static void display_fill_pixels(uint16_t color, uint32_t count) {
    uint8_t hi = color >> 8, lo = color & 0xFF;
    uint8_t buf[2] = {hi, lo};
    gpio_put(DISPLAY_DC, 1);
    gpio_put(DISPLAY_CS, 0);
    while (count--) spi_write_blocking(DISPLAY_SPI, buf, 2);
    gpio_put(DISPLAY_CS, 1);
}

// Write raw byte buffer (pre-formatted RGB565 pairs)
static void display_write_buf(const uint8_t* buf, uint32_t len) {
    gpio_put(DISPLAY_DC, 1);
    gpio_put(DISPLAY_CS, 0);
    spi_write_blocking(DISPLAY_SPI, buf, len);
    gpio_put(DISPLAY_CS, 1);
}

// ─── Init ────────────────────────────────────────────────────────────────────

void display_init(void) {
    // SPI at 40 MHz
    spi_init(DISPLAY_SPI, 40 * 1000 * 1000);
    gpio_set_function(DISPLAY_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(DISPLAY_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(DISPLAY_MISO, GPIO_FUNC_SPI);

    gpio_init(DISPLAY_CS);  gpio_set_dir(DISPLAY_CS,  GPIO_OUT); gpio_put(DISPLAY_CS,  1);
    gpio_init(DISPLAY_DC);  gpio_set_dir(DISPLAY_DC,  GPIO_OUT); gpio_put(DISPLAY_DC,  0);
    gpio_init(DISPLAY_RST); gpio_set_dir(DISPLAY_RST, GPIO_OUT); gpio_put(DISPLAY_RST, 1);
    gpio_init(DISPLAY_BL);  gpio_set_dir(DISPLAY_BL,  GPIO_OUT); gpio_put(DISPLAY_BL,  1);

    // Hardware reset
    gpio_put(DISPLAY_RST, 0); sleep_ms(10);
    gpio_put(DISPLAY_RST, 1); sleep_ms(10);

    // Software reset + sleep out
    display_cmd(0x01); sleep_ms(5);
    display_cmd(0x11); sleep_ms(120);

    // Extended command set
    display_cmd(0xCB); { uint8_t d[]={0x39,0x2C,0x00,0x34,0x02}; display_data(d,5); }
    display_cmd(0xCF); { uint8_t d[]={0x00,0xC1,0x30};            display_data(d,3); }
    display_cmd(0xE8); { uint8_t d[]={0x85,0x00,0x78};            display_data(d,3); }
    display_cmd(0xEA); { uint8_t d[]={0x00,0x00};                 display_data(d,2); }
    display_cmd(0xED); { uint8_t d[]={0x64,0x03,0x12,0x81};       display_data(d,4); }
    display_cmd(0xF7); display_data1(0x20);

    // Power control
    display_cmd(0xC0); display_data1(0x23);
    display_cmd(0xC1); display_data1(0x10);
    display_cmd(0xC5); { uint8_t d[]={0x3E,0x28}; display_data(d,2); }
    display_cmd(0xC7); display_data1(0x86);

    // Memory access — landscape 320x240, BGR color order
    display_cmd(0x36); display_data1(0x28);  // MV=1, BGR=1
    display_cmd(0x3A); display_data1(0x55);  // 16-bit color (RGB565)

    // Frame rate ~60 Hz
    display_cmd(0xB1); { uint8_t d[]={0x00,0x18}; display_data(d,2); }

    // Display function control
    display_cmd(0xB6); { uint8_t d[]={0x08,0x82,0x27}; display_data(d,3); }

    // Gamma
    display_cmd(0xF2); display_data1(0x00);
    display_cmd(0x26); display_data1(0x01);
    display_cmd(0xE0); {
        uint8_t d[]={0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,
                     0x37,0x07,0x10,0x03,0x0E,0x09,0x00};
        display_data(d,15);
    }
    display_cmd(0xE1); {
        uint8_t d[]={0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,
                     0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F};
        display_data(d,15);
    }

    // Wake up
    display_cmd(0x29);   // Display ON

    // Clear to black
    display_fill(COLOR_BLACK);
}

// ─── Drawing primitives ──────────────────────────────────────────────────────

void display_fill(uint16_t color) {
    display_set_addr_window(0, 0, DISPLAY_W - 1, DISPLAY_H - 1);
    display_fill_pixels(color, (uint32_t)DISPLAY_W * DISPLAY_H);
}

void display_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (w <= 0 || h <= 0) return;
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x >= DISPLAY_W || y >= DISPLAY_H) return;
    if (x + w > DISPLAY_W) w = DISPLAY_W - x;
    if (y + h > DISPLAY_H) h = DISPLAY_H - y;
    display_set_addr_window(x, y, x + w - 1, y + h - 1);
    display_fill_pixels(color, (uint32_t)w * h);
}

void display_draw_pixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || y < 0 || x >= DISPLAY_W || y >= DISPLAY_H) return;
    display_set_addr_window(x, y, x, y);
    display_fill_pixels(color, 1);
}

// ─── Text rendering ──────────────────────────────────────────────────────────

// Draw one character; returns new x position
static int16_t display_draw_char(int16_t x, int16_t y,
                                  char c, uint16_t fg, uint16_t bg) {
    if (x + FONT_W > DISPLAY_W || y + FONT_H > DISPLAY_H) return x + FONT_W + 1;
    if (c < FONT_FIRST || c > FONT_LAST) c = '?';

    const uint8_t* glyph = FONT_5x7[c - FONT_FIRST];

    // Build pixel buffer for this character (5 cols x 7 rows = 35 pixels, 70 bytes)
    uint8_t buf[FONT_W * FONT_H * 2];
    uint16_t idx = 0;
    for (int row = 0; row < FONT_H; row++) {
        for (int col = 0; col < FONT_W; col++) {
            uint16_t color = (glyph[col] & (1 << row)) ? fg : bg;
            buf[idx++] = color >> 8;
            buf[idx++] = color & 0xFF;
        }
    }
    display_set_addr_window(x, y, x + FONT_W - 1, y + FONT_H - 1);
    display_write_buf(buf, sizeof(buf));

    // 1-pixel column gap
    if (x + FONT_W < DISPLAY_W) {
        display_set_addr_window(x + FONT_W, y, x + FONT_W, y + FONT_H - 1);
        display_fill_pixels(bg, FONT_H);
    }
    return x + FONT_W + 1;
}

void display_draw_text(int16_t x, int16_t y,
                        const char* text, uint16_t fg, uint16_t bg) {
    while (*text && x < DISPLAY_W) {
        x = display_draw_char(x, y, *text++, fg, bg);
    }
}

void display_flush(void) {
    // No-op: single-buffer mode. Add DMA double-buffering here later.
}
