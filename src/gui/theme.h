#pragma once
#include <stdint.h>

// HorizonOS theme — Nintendo Switch inspired
// Colors in RGB565

#define THEME_BG           0x0C19   // Dark navy  #181830
#define THEME_BG_CARD      0x1A53   // Card bg    #30A0
#define THEME_ACCENT       0x4DF6   // Cyan/teal  #4ADAEC  (Switch blue-ish)
#define THEME_ACCENT2      0xFD20   // Orange     #FF6400  (Switch orange)
#define THEME_TEXT         0xFFFF   // White
#define THEME_TEXT_MUTED   0xC618   // Gray
#define THEME_BAR_BG       0x0841   // Status bar bg

// Font sizes (bitmap, in pixels)
#define FONT_SM   8
#define FONT_MD   12
#define FONT_LG   16

// Layout constants
#define STATUS_BAR_H   20
#define DOCK_H         48
#define CARD_W         80
#define CARD_H         80
#define CARD_RADIUS    12
#define CARD_GAP       16

