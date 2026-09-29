#include "display.h"
#include "font5x7.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdint.h>

// Shared with main_sim.c
SDL_Renderer* g_renderer = NULL;
SDL_Texture*  g_texture  = NULL;

// RGB565 framebuffer
static uint16_t fb[DISPLAY_W * DISPLAY_H];

// Called from main_sim.c after SDL_Init
void display_sim_set_sdl(SDL_Renderer* r, SDL_Texture* t) {
    g_renderer = r;
    g_texture  = t;
}

static inline void set_px(int x, int y, uint16_t c) {
    if ((unsigned)x < DISPLAY_W && (unsigned)y < DISPLAY_H)
        fb[y * DISPLAY_W + x] = c;
}

void display_init(void)  { memset(fb, 0, sizeof(fb)); }

void display_fill(uint16_t c) {
    for (int i = 0; i < DISPLAY_W * DISPLAY_H; i++) fb[i] = c;
}

void display_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c) {
    for (int ry = y; ry < y+h; ry++)
        for (int rx = x; rx < x+w; rx++)
            set_px(rx, ry, c);
}

void display_draw_pixel(int16_t x, int16_t y, uint16_t c) { set_px(x, y, c); }

void display_draw_rect_border(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint8_t t, uint16_t c) {
    display_draw_rect(x,     y,     w, t,       c);
    display_draw_rect(x,     y+h-t, w, t,       c);
    display_draw_rect(x,     y+t,   t, h-2*t,   c);
    display_draw_rect(x+w-t, y+t,   t, h-2*t,   c);
}

void display_draw_text(int16_t x, int16_t y, const char* text, uint16_t fg, uint16_t bg) {
    while (*text && x < DISPLAY_W) {
        char ch = *text++;
        if (ch < FONT_FIRST || ch > FONT_LAST) ch = '?';
        const uint8_t* g = FONT_5x7[ch - FONT_FIRST];
        for (int row = 0; row < FONT_H; row++)
            for (int col = 0; col < FONT_W; col++)
                set_px(x+col, y+row, (g[col] & (1<<row)) ? fg : bg);
        for (int row = 0; row < FONT_H; row++) set_px(x+FONT_W, y+row, bg);
        x += FONT_W + 1;
    }
}

void display_draw_text_scaled(int16_t x, int16_t y, const char* text,
                               uint16_t fg, uint16_t bg, uint8_t scale) {
    if (!scale) scale = 1;
    int cw = FONT_W * scale, ch = FONT_H * scale;
    while (*text && x + cw <= DISPLAY_W) {
        char c = *text++;
        if (c < FONT_FIRST || c > FONT_LAST) c = '?';
        const uint8_t* g = FONT_5x7[c - FONT_FIRST];
        for (int row = 0; row < FONT_H; row++)
            for (int sr = 0; sr < scale; sr++)
                for (int col = 0; col < FONT_W; col++)
                    for (int sc = 0; sc < scale; sc++)
                        set_px(x + col*scale + sc, y + row*scale + sr,
                               (g[col] & (1<<row)) ? fg : bg);
        for (int py = 0; py < ch; py++)
            for (int px = 0; px < scale; px++)
                set_px(x + cw + px, y + py, bg);
        x += cw + scale;
    }
}

void display_flush(void) {
    if (!g_renderer || !g_texture) return;
    // Convert RGB565 -> RGBA8888
    static uint32_t pixels[DISPLAY_H * DISPLAY_W];
    for (int i = 0; i < DISPLAY_W * DISPLAY_H; i++) {
        uint16_t c = fb[i];
        uint8_t r = ((c >> 11) & 0x1F) << 3;
        uint8_t g = ((c >> 5)  & 0x3F) << 2;
        uint8_t b = ( c        & 0x1F) << 3;
        pixels[i] = (0xFFu << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }
    SDL_UpdateTexture(g_texture, NULL, pixels, DISPLAY_W * 4);
    SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);
    SDL_RenderPresent(g_renderer);
}
