#pragma once
#include <stdint.h>

// ── HorizonOS color palette (RGB565) ─────────────────────────────────────────
// Inspired by Nintendo Switch Horizon OS dark theme.

#define RGB(r,g,b) ((uint16_t)(((r&0xF8u)<<8)|((g&0xFCu)<<3)|((b)>>3)))

// Backgrounds
#define THEME_BG          RGB(18,  18,  36)   // main background  — deep navy
#define THEME_BG_DARKER   RGB(10,  10,  22)   // status/action bar
#define THEME_BG_CARD     RGB(30,  30,  58)   // unselected card overlay (unused)

// Accent colors
#define THEME_ACCENT      RGB(100, 210, 245)  // cyan highlight (Switch blue-ish)
#define THEME_ACCENT2     RGB(255, 100,  30)  // orange (Switch accent 2)
#define THEME_GLOW        RGB(60,  160, 200)  // softer glow ring inner

// Text
#define THEME_TEXT        RGB(255, 255, 255)  // primary white
#define THEME_TEXT_DIM    RGB(140, 140, 160)  // muted / secondary
#define THEME_TEXT_SEL    RGB(100, 210, 245)  // selected label (= accent)

// App card colors (each app has its own identity color)
#define APP_COLOR_FILES   RGB(230, 110,  20)  // warm orange
#define APP_COLOR_NET     RGB( 20, 100, 220)  // vivid blue
#define APP_COLOR_SHELL   RGB( 20, 140,  80)  // terminal green
#define APP_COLOR_SETT    RGB( 50,  60,  75)  // dark slate

// Layout
#define STATUS_BAR_H   22    // top status bar height
#define ACTION_BAR_H   18    // bottom action bar height
#define CARD_W         72    // app card width
#define CARD_H         72    // app card height
#define CARD_R          8    // rounded corner radius
#define CARD_GAP       18    // horizontal gap between cards
#define CARD_Y         75    // card row top Y
#define LABEL_Y       157    // label row top Y (below cards)
#define DOTS_Y        205    // pagination dots Y
#define LABEL_SCALE     2    // font scale for card labels (2x = 10x14 px)
