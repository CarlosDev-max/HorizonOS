#include "display.h"
#include "font5x7.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <string.h>

// ─── SPI primitives ──────────────────────────────────────────────────────────

static inline void display_cmd(uint8_t cmd) {
    gpio_put(DISPLAY_DC, 0);
    gpio_put(DISPLAY_CS, 0);
    spi_write_blocking(DISPLAY_SPI, &cmd, 1);
    gpio_put(DISPLAY_CS, 1);
}
static inline void display_data(const uint8_t* d, size_t len) {
    gpio_put(DISPLAY_DC, 1);
    gpio_put(DISPLAY_CS, 0);
    spi_write_blocking(DISPLAY_SPI, d, len);
    gpio_put(DISPLAY_CS, 1);
}
static inline void display_data1(uint8_t b) { display_data(&b, 1); }

static void display_set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t d[4];
    display_cmd(0x2A);
    d[0]=x0>>8; d[1]=x0; d[2]=x1>>8; d[3]=x1; display_data(d,4);
    display_cmd(0x2B);
    d[0]=y0>>8; d[1]=y0; d[2]=y1>>8; d[3]=y1; display_data(d,4);
    display_cmd(0x2C);
}

// Fill n pixels with same color (CS stays toggled per call)
static void fill_px(uint16_t color, uint32_t n) {
    uint8_t buf[2] = {color>>8, color&0xFF};
    gpio_put(DISPLAY_DC, 1);
    gpio_put(DISPLAY_CS, 0);
    while (n--) spi_write_blocking(DISPLAY_SPI, buf, 2);
    gpio_put(DISPLAY_CS, 1);
}
// Write raw byte buffer (CS must be managed by caller)
static inline void push_raw(const uint8_t* buf, uint32_t len) {
    spi_write_blocking(DISPLAY_SPI, buf, len);
}

// ─── Init ────────────────────────────────────────────────────────────────────

void display_init(void) {
    spi_init(DISPLAY_SPI, 40*1000*1000);
    gpio_set_function(DISPLAY_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(DISPLAY_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(DISPLAY_MISO, GPIO_FUNC_SPI);

    gpio_init(DISPLAY_CS);  gpio_set_dir(DISPLAY_CS,  GPIO_OUT); gpio_put(DISPLAY_CS,  1);
    gpio_init(DISPLAY_DC);  gpio_set_dir(DISPLAY_DC,  GPIO_OUT);
    gpio_init(DISPLAY_RST); gpio_set_dir(DISPLAY_RST, GPIO_OUT); gpio_put(DISPLAY_RST, 1);
    gpio_init(DISPLAY_BL);  gpio_set_dir(DISPLAY_BL,  GPIO_OUT); gpio_put(DISPLAY_BL,  1);

    gpio_put(DISPLAY_RST, 0); sleep_ms(10);
    gpio_put(DISPLAY_RST, 1); sleep_ms(10);

    display_cmd(0x01); sleep_ms(5);
    display_cmd(0x11); sleep_ms(120);

    display_cmd(0xCB); { uint8_t d[]={0x39,0x2C,0x00,0x34,0x02}; display_data(d,5); }
    display_cmd(0xCF); { uint8_t d[]={0x00,0xC1,0x30};            display_data(d,3); }
    display_cmd(0xE8); { uint8_t d[]={0x85,0x00,0x78};            display_data(d,3); }
    display_cmd(0xEA); { uint8_t d[]={0x00,0x00};                 display_data(d,2); }
    display_cmd(0xED); { uint8_t d[]={0x64,0x03,0x12,0x81};       display_data(d,4); }
    display_cmd(0xF7); display_data1(0x20);
    display_cmd(0xC0); display_data1(0x23);
    display_cmd(0xC1); display_data1(0x10);
    display_cmd(0xC5); { uint8_t d[]={0x3E,0x28}; display_data(d,2); }
    display_cmd(0xC7); display_data1(0x86);
    display_cmd(0x36); display_data1(0x28); // landscape, BGR
    display_cmd(0x3A); display_data1(0x55); // RGB565
    display_cmd(0xB1); { uint8_t d[]={0x00,0x18}; display_data(d,2); }
    display_cmd(0xB6); { uint8_t d[]={0x08,0x82,0x27}; display_data(d,3); }
    display_cmd(0xF2); display_data1(0x00);
    display_cmd(0x26); display_data1(0x01);
    display_cmd(0xE0); {
        uint8_t d[]={0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00};
        display_data(d,15);
    }
    display_cmd(0xE1); {
        uint8_t d[]={0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F};
        display_data(d,15);
    }
    display_cmd(0x29);
    display_fill(COLOR_BLACK);
}

// ─── Primitives ──────────────────────────────────────────────────────────────

void display_fill(uint16_t color) {
    display_set_addr(0, 0, DISPLAY_W-1, DISPLAY_H-1);
    fill_px(color, (uint32_t)DISPLAY_W * DISPLAY_H);
}

void display_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (w<=0||h<=0) return;
    if (x<0){w+=x;x=0;} if (y<0){h+=y;y=0;}
    if (x>=DISPLAY_W||y>=DISPLAY_H) return;
    if (x+w>DISPLAY_W) w=DISPLAY_W-x;
    if (y+h>DISPLAY_H) h=DISPLAY_H-y;
    display_set_addr(x, y, x+w-1, y+h-1);
    fill_px(color, (uint32_t)w*h);
}

void display_draw_pixel(int16_t x, int16_t y, uint16_t color) {
    if (x<0||y<0||x>=DISPLAY_W||y>=DISPLAY_H) return;
    display_set_addr(x,y,x,y);
    fill_px(color,1);
}

void display_draw_rect_border(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint8_t t, uint16_t color) {
    display_draw_rect(x,     y,     w, t, color);
    display_draw_rect(x,     y+h-t, w, t, color);
    display_draw_rect(x,     y+t,   t, h-2*t, color);
    display_draw_rect(x+w-t, y+t,   t, h-2*t, color);
}

// ─── Text ────────────────────────────────────────────────────────────────────

static int16_t draw_char(int16_t x, int16_t y, char c, uint16_t fg, uint16_t bg) {
    if (x+FONT_W>DISPLAY_W || y+FONT_H>DISPLAY_H) return x+FONT_W+1;
    if (c<FONT_FIRST||c>FONT_LAST) c='?';
    const uint8_t* g = FONT_5x7[c-FONT_FIRST];
    uint8_t buf[FONT_W*FONT_H*2];
    uint16_t i=0;
    for (int row=0;row<FONT_H;row++)
        for (int col=0;col<FONT_W;col++) {
            uint16_t px = (g[col]&(1<<row)) ? fg : bg;
            buf[i++]=px>>8; buf[i++]=px&0xFF;
        }
    display_set_addr(x,y,x+FONT_W-1,y+FONT_H-1);
    display_data(buf,sizeof(buf));
    if (x+FONT_W<DISPLAY_W) { display_set_addr(x+FONT_W,y,x+FONT_W,y+FONT_H-1); fill_px(bg,FONT_H); }
    return x+FONT_W+1;
}

void display_draw_text(int16_t x, int16_t y, const char* text, uint16_t fg, uint16_t bg) {
    while (*text && x<DISPLAY_W) x = draw_char(x,y,*text++,fg,bg);
}

void display_draw_text_scaled(int16_t x, int16_t y, const char* text,
                               uint16_t fg, uint16_t bg, uint8_t scale) {
    if (!scale) scale=1;
    if (scale==1) { display_draw_text(x,y,text,fg,bg); return; }

    const int cw = FONT_W * scale;
    const int ch = FONT_H * scale;

    while (*text && x+cw <= DISPLAY_W) {
        char c = *text++;
        if (c<FONT_FIRST||c>FONT_LAST) c='?';
        const uint8_t* g = FONT_5x7[c-FONT_FIRST];

        // row buffer: cw pixels = FONT_W*scale pixels wide
        uint8_t row_buf[FONT_W * 4 * 2]; // max scale=4 → 40 bytes

        display_set_addr(x, y, x+cw-1, y+ch-1);
        gpio_put(DISPLAY_DC, 1);
        gpio_put(DISPLAY_CS, 0);

        for (int row=0; row<FONT_H; row++) {
            // build one scaled row
            uint16_t idx=0;
            for (int col=0; col<FONT_W; col++) {
                uint16_t px = (g[col]&(1<<row)) ? fg : bg;
                uint8_t hi=px>>8, lo=px&0xFF;
                for (int sc=0; sc<scale; sc++) { row_buf[idx++]=hi; row_buf[idx++]=lo; }
            }
            for (int sr=0; sr<scale; sr++) push_raw(row_buf, cw*2);
        }
        gpio_put(DISPLAY_CS, 1);

        // gap column(s)
        if (x+cw+scale <= DISPLAY_W)
            display_draw_rect(x+cw, y, scale, ch, bg);

        x += cw + scale;
    }
}

void display_flush(void) {}
