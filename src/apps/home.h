#pragma once
#include <stdint.h>

void app_home_launch(void);
void app_home_draw(void);
void app_home_touch(int16_t x, int16_t y);
void app_home_tick(void);   // call each frame for animation
