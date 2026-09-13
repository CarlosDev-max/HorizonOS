#include "window_manager.h"
#include "hal/display.h"
#include "hal/sound.h"
#include <string.h>

static Window* stack[MAX_WINDOWS];
static int8_t top = -1;

void wm_init(void) {
    top = -1;
}

void wm_push(Window* win) {
    if (top >= MAX_WINDOWS - 1) return;
    win->active = true;
    stack[++top] = win;
    sound_play_click();
}

void wm_pop(void) {
    if (top < 0) return;
    stack[top]->active = false;
    top--;
    sound_play_click();
}

void wm_draw(void) {
    if (top < 0) return;
    if (stack[top]->on_draw) stack[top]->on_draw();
}

void wm_dispatch_touch(int16_t x, int16_t y) {
    if (top < 0) return;
    if (stack[top]->on_touch) stack[top]->on_touch(x, y);
}

Window* wm_current(void) {
    if (top < 0) return NULL;
    return stack[top];
}

