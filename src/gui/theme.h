#pragma once
#include <stdint.h>

// ── HorizonOS color palette (RGB565) ─────────────────────────────────────────
// Inspired by Nintendo Switch Horizon OS dark theme.

// Guard against redefinition (display.h also defines this)
#ifndef RGB
#define RGB(r,g,b) ((uint16_t)(((r&0xF8u)<<8)|((g&0xFCu)<<3)|((b)>>3)))
#endif

// Backgrounds
#define THEME_BG          RGB(18,  18,  36)   // deep navy
#define THEME_BG_DARKER   RGB(10,  10,  22)   // status/action bar
#define THEME_BG_CARD     RGB(30,  30,  58)   // card overlay (future)

// Accent
#define THEME_ACCENT      RGB(100, 210, 245)  // cyan (Switch blue-ish)
#define THEME_ACCENT2     RGB(255, 100,  30)  // orange (Switch accent 2)
#define THEME_GLOW        RGB(60,  160, 200)  // softer glow ring

// Text
#define THEME_TEXT        RGB(255, 255, 255)  // primary white
#define THEME_TEXT_DIM    RGB(140, 140, 160)  // muted / secondary
#define THEME_TEXT_SEL    RGB(100, 210, 245)  // selected label = accent

// App card identity colors
#define APP_COLOR_FILES   RGB(230, 110,  20)
#define APP_COLOR_NET     RGB( 20, 100, 220)
#define APP_COLOR_SHELL   RGB( 20, 140,  80)
#define APP_COLOR_SETT    RGB( 50,  60,  75)

// Layout constants
#define STATUS_BAR_H   22
#define ACTION_BAR_H   18
#define CARD_W         72
#define CARD_H         72
#define CARD_R          8
#define CARD_GAP       18
#define CARD_Y         75
#define LABEL_Y       157
#define DOTS_Y        205
#define LABEL_SCALE     2
