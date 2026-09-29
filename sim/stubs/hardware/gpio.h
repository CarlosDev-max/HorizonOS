#pragma once
#include <stdint.h>
#include <stdbool.h>
#define GPIO_OUT      1
#define GPIO_IN       0
#define GPIO_FUNC_SPI 1
#define GPIO_FUNC_PWM 2
static inline void gpio_init(uint32_t p)                          { (void)p; }
static inline void gpio_set_dir(uint32_t p, int d)                { (void)p;(void)d; }
static inline void gpio_put(uint32_t p, bool v)                   { (void)p;(void)v; }
static inline bool gpio_get(uint32_t p)                           { (void)p; return false; }
static inline void gpio_pull_up(uint32_t p)                       { (void)p; }
static inline void gpio_set_function(uint32_t p, int f)           { (void)p;(void)f; }
