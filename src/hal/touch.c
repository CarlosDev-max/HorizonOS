#include "touch.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "display.h"

void touch_init(void) {
    gpio_init(TOUCH_CS);
    gpio_set_dir(TOUCH_CS, GPIO_OUT);
    gpio_put(TOUCH_CS, 1);
}

bool touch_read(TouchEvent* ev) {
    // TODO: read XPT2046 via SPI, calibrate to display coords
    ev->pressed = false;
    ev->x = 0;
    ev->y = 0;
    return false;
}

