
/* HorizonOS Windows Simulator — GDI rendering, no SDL2 required.
   Standalone .exe: only needs gdi32.dll + user32.dll (Windows built-ins).

   Controls:
     Left / Right arrow  : navigate
     Enter or Space      : open selected app
     Escape              : quit
*/

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601   // Windows 7+
#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* ── Display geometry ────────────────────────────────────────────────────── */
#define DISPLAY_W  320
#define DISPLAY_H  240
#define SCALE      2          // window = 640×480

/* ── Framebuffer (RGB565) — written by all display_* functions ───────────── */
static uint16_t fb[DISPLAY_H * DISPLAY_W];

/* ── OS API forward declarations ─────────────────────────────────────────── */
extern void wm_init(void);
extern void sound_init(void);
extern void input_init(void);
extern void app_home_launch(void);
extern void app_home_tick(void);

/* ── Input queue (input_sim.c) ───────────────────────────────────────────── */
extern void sim_input_push(int ev);
#define INPUT_LEFT   1
#define INPUT_RIGHT  2
#define INPUT_SELECT 5

/* ── Font (same as HAL) ──────────────────────────────────────────────────── */
#include "font5x7.h"
#include "display.h"   // for RGB macro and constants

/* ── RGB565 helpers ──────────────────────────────────────────────────────── */
static inline void set_px(int x, int y, uint16_t c) {
    if ((unsigned)x < DISPLAY_W && (unsigned)y < DISPLAY_H)
        fb[y * DISPLAY_W + x] = c;
}

/* ═══ display_*.c replacement for Windows simulator ═══════════════════════ */

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
    display_draw_rect(x,     y,     w, t,     c);
    display_draw_rect(x,     y+h-t, w, t,     c);
    display_draw_rect(x,     y+t,   t, h-2*t, c);
    display_draw_rect(x+w-t, y+t,   t, h-2*t, c);
}

void display_draw_text(int16_t x, int16_t y, const char* txt, uint16_t fg, uint16_t bg) {
    while (*txt && x < DISPLAY_W) {
        char ch = *txt++;
        if (ch < FONT_FIRST || ch > FONT_LAST) ch = '?';
        const uint8_t* g = FONT_5x7[ch - FONT_FIRST];
        for (int row = 0; row < FONT_H; row++)
            for (int col = 0; col < FONT_W; col++)
                set_px(x+col, y+row, (g[col] & (1<<row)) ? fg : bg);
        for (int row = 0; row < FONT_H; row++) set_px(x+FONT_W, y+row, bg);
        x += FONT_W + 1;
    }
}

void display_draw_text_scaled(int16_t x, int16_t y, const char* txt,
                               uint16_t fg, uint16_t bg, uint8_t scale) {
    if (!scale) scale = 1;
    int cw = FONT_W*scale, ch = FONT_H*scale;
    while (*txt && x+cw <= DISPLAY_W) {
        char c = *txt++;
        if (c < FONT_FIRST || c > FONT_LAST) c = '?';
        const uint8_t* g = FONT_5x7[c - FONT_FIRST];
        for (int row = 0; row < FONT_H; row++)
            for (int sr = 0; sr < scale; sr++)
                for (int col = 0; col < FONT_W; col++)
                    for (int sc2 = 0; sc2 < scale; sc2++)
                        set_px(x+col*scale+sc2, y+row*scale+sr,
                               (g[col]&(1<<row)) ? fg : bg);
        for (int py = 0; py < ch; py++)
            for (int px = 0; px < scale; px++)
                set_px(x+cw+px, y+py, bg);
        x += cw + scale;
    }
}

/* ── GDI flush ───────────────────────────────────────────────────────────── */
static HWND g_hwnd;
static uint32_t g_pixels[DISPLAY_H * DISPLAY_W]; // BGR32 for GDI
static BITMAPINFO g_bmi;

void display_flush(void) {
    for (int i = 0; i < DISPLAY_W * DISPLAY_H; i++) {
        uint16_t c = fb[i];
        uint8_t r = ((c >> 11) & 0x1F) << 3;
        uint8_t gv = ((c >> 5)  & 0x3F) << 2;
        uint8_t b = ( c         & 0x1F) << 3;
        g_pixels[i] = ((uint32_t)r << 16) | ((uint32_t)gv << 8) | b;
    }
    InvalidateRect(g_hwnd, NULL, FALSE);
    UpdateWindow(g_hwnd);
}

/* ── Windows message handler ─────────────────────────────────────────────── */
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            StretchDIBits(hdc, 0, 0, DISPLAY_W*SCALE, DISPLAY_H*SCALE,
                          0, 0, DISPLAY_W, DISPLAY_H,
                          g_pixels, &g_bmi, DIB_RGB_COLORS, SRCCOPY);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_KEYDOWN:
            switch (wp) {
                case VK_LEFT:   sim_input_push(INPUT_LEFT);   break;
                case VK_RIGHT:  sim_input_push(INPUT_RIGHT);  break;
                case VK_RETURN:
                case VK_SPACE:  sim_input_push(INPUT_SELECT); break;
                case VK_ESCAPE: PostQuitMessage(0);            break;
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
}

/* ── WinMain ─────────────────────────────────────────────────────────────── */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmd, int nShow) {
    (void)hPrev; (void)cmd;

    g_bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    g_bmi.bmiHeader.biWidth       = DISPLAY_W;
    g_bmi.bmiHeader.biHeight      = -DISPLAY_H;
    g_bmi.bmiHeader.biPlanes      = 1;
    g_bmi.bmiHeader.biBitCount    = 32;
    g_bmi.bmiHeader.biCompression = BI_RGB;

    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "HorizonOS";
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    RECT rc = {0, 0, DISPLAY_W*SCALE, DISPLAY_H*SCALE};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hwnd = CreateWindowA("HorizonOS",
        "HorizonOS Simulator  |  Arrow keys = navigate  |  Enter = open  |  Esc = quit",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, hInst, NULL);

    if (!g_hwnd) return 1;

    /* Boot OS */
    display_init();
    sound_init();
    input_init();
    wm_init();
    app_home_launch();

    /* Main loop */
    MSG msg2;
    DWORD last = GetTickCount();
    while (1) {
        while (PeekMessage(&msg2, NULL, 0, 0, PM_REMOVE)) {
            if (msg2.message == WM_QUIT) return 0;
            TranslateMessage(&msg2);
            DispatchMessage(&msg2);
        }
        DWORD now = GetTickCount();
        if (now - last >= 16) {
            app_home_tick();
            last = now;
        }
        Sleep(1);
    }
    return 0;
}
