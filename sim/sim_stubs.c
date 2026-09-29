// Pico SDK function implementations for the simulator (uses SDL2 for timing)
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t absolute_time_t;

void stdio_init_all(void) {}
void tight_loop_contents(void) {}
void sleep_ms(uint32_t ms) { SDL_Delay(ms); }

absolute_time_t get_absolute_time(void) {
    return (absolute_time_t)SDL_GetTicks64();
}
uint32_t to_ms_since_boot(absolute_time_t t) {
    return (uint32_t)t;
}
