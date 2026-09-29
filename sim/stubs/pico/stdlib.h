#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PICO_ERROR_TIMEOUT (-1)

typedef uint64_t absolute_time_t;

void            stdio_init_all(void);
void            tight_loop_contents(void);
void            sleep_ms(uint32_t ms);
uint32_t        to_ms_since_boot(absolute_time_t t);
absolute_time_t get_absolute_time(void);

static inline int getchar_timeout_us(uint32_t us) { (void)us; return PICO_ERROR_TIMEOUT; }
