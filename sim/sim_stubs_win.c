/* Pico SDK stub implementations for Windows GDI simulator.
   Uses Windows API instead of SDL2 for timing. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdio.h>

typedef uint64_t absolute_time_t;

void stdio_init_all(void)     {}
void tight_loop_contents(void) {}

void sleep_ms(uint32_t ms) { Sleep(ms); }

absolute_time_t get_absolute_time(void) {
    return (absolute_time_t)GetTickCount64();
}
uint32_t to_ms_since_boot(absolute_time_t t) {
    return (uint32_t)t;
}
