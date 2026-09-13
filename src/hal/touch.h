#pragma once
#include <stdint.h>
#include <stdbool.h>

// XPT2046 SPI (shares SPI0 with display, separate CS)
#define TOUCH_CS   15

typedef struct {
    int16_t x;
    int16_t y;
    bool    pressed;
} TouchEvent;

void touch_init(void);
bool touch_read(TouchEvent* ev);

