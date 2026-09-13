#pragma once
#include <stdint.h>
#include <stdbool.h>

#define MAX_WINDOWS 4

typedef void (*draw_fn_t)(void);
typedef void (*touch_fn_t)(int16_t x, int16_t y);

typedef struct {
    const char* title;
    draw_fn_t   on_draw;
    touch_fn_t  on_touch;
    bool        active;
} Window;

void wm_init(void);
void wm_push(Window* win);
void wm_pop(void);
void wm_draw(void);
void wm_dispatch_touch(int16_t x, int16_t y);
Window* wm_current(void);

