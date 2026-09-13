#include "display.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

static void display_cmd(uint8_t cmd) {
    gpio_put(DISPLAY_DC, 0);
    gpio_put(DISPLAY_CS, 0);
    spi_write_blocking(DISPLAY_SPI, &cmd, 1);
    gpio_put(DISPLAY_CS, 1);
}

static void display_data(uint8_t* data, size_t len) {
    gpio_put(DISPLAY_DC, 1);
    gpio_put(DISPLAY_CS, 0);
    spi_write_blocking(DISPLAY_SPI, data, len);
    gpio_put(DISPLAY_CS, 1);
}

void display_init(void) {
    spi_init(DISPLAY_SPI, 40 * 1000 * 1000);
    gpio_set_function(DISPLAY_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(DISPLAY_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(DISPLAY_MISO, GPIO_FUNC_SPI);

    gpio_init(DISPLAY_CS);  gpio_set_dir(DISPLAY_CS,  GPIO_OUT); gpio_put(DISPLAY_CS, 1);
    gpio_init(DISPLAY_DC);  gpio_set_dir(DISPLAY_DC,  GPIO_OUT);
    gpio_init(DISPLAY_RST); gpio_set_dir(DISPLAY_RST, GPIO_OUT);
    gpio_init(DISPLAY_BL);  gpio_set_dir(DISPLAY_BL,  GPIO_OUT); gpio_put(DISPLAY_BL, 1);

    // Hardware reset
    gpio_put(DISPLAY_RST, 0); sleep_ms(10);
    gpio_put(DISPLAY_RST, 1); sleep_ms(120);

    // ILI9341 init sequence (minimal)
    display_cmd(0x01); sleep_ms(5);   // Software reset
    display_cmd(0x11); sleep_ms(120); // Sleep out
    display_cmd(0x3A); uint8_t px = 0x55; display_data(&px, 1); // 16-bit color
    display_cmd(0x36); uint8_t mx = 0x48; display_data(&mx, 1); // Memory access
    display_cmd(0x29);                // Display on
}

void display_fill(uint16_t color) {
    // TODO: set addr window full screen, DMA fill
}

void display_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    // TODO: set addr window, fill
}

void display_draw_text(int16_t x, int16_t y, const char* text, uint16_t fg, uint16_t bg) {
    // TODO: bitmap font renderer
}

void display_flush(void) {
    // TODO: DMA flush when double-buffering
}

