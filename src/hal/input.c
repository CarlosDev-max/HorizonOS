#include "input.h"
#include "touch.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define DEBOUNCE_MS 80

typedef struct {
    uint8_t  pin;
    bool     last;
    uint32_t last_ms;
} BtnState;

static BtnState btns[] = {
    { BTN_LEFT,  false, 0 },
    { BTN_RIGHT, false, 0 },
    { BTN_OK,    false, 0 },
    { BTN_BACK,  false, 0 },
};
#define N_BTNS (sizeof(btns)/sizeof(btns[0]))

void input_init(void) {
    for (uint8_t i = 0; i < N_BTNS; i++) {
        gpio_init(btns[i].pin);
        gpio_set_dir(btns[i].pin, GPIO_IN);
        gpio_pull_up(btns[i].pin);
    }
    touch_init();
}

InputEvent input_poll(void) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    // ── GPIO buttons (Wokwi / fallback) ──────────────────────────────────
    static const InputEvent btn_events[] = {
        INPUT_LEFT, INPUT_RIGHT, INPUT_SELECT, INPUT_BACK
    };
    for (uint8_t i = 0; i < N_BTNS; i++) {
        bool pressed = !gpio_get(btns[i].pin);
        if (pressed && !btns[i].last && (now - btns[i].last_ms) > DEBOUNCE_MS) {
            btns[i].last    = true;
            btns[i].last_ms = now;
            return btn_events[i];
        }
        if (!pressed) btns[i].last = false;
    }

    // ── XPT2046 touch (real hardware) ────────────────────────────────────
    static bool touch_was_pressed = false;  // single declaration
    TouchEvent te;
    if (touch_read(&te) && te.pressed) {
        if (!touch_was_pressed) {
            touch_was_pressed = true;
            return INPUT_SELECT;
        }
    } else {
        touch_was_pressed = false;
    }

    return INPUT_NONE;
}
