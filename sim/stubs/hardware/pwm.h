#pragma once
#include <stdint.h>
static inline uint32_t pwm_gpio_to_slice_num(uint32_t p)          { (void)p; return 0; }
static inline uint32_t pwm_gpio_to_channel(uint32_t p)            { (void)p; return 0; }
static inline void pwm_set_wrap(uint32_t s, uint32_t w)           { (void)s;(void)w; }
static inline void pwm_set_chan_level(uint32_t s, uint32_t c, uint16_t l) { (void)s;(void)c;(void)l; }
static inline void pwm_set_enabled(uint32_t s, bool e)            { (void)s;(void)e; }
