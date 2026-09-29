#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

// Forward declarations from display_sim.c
void display_sim_set_sdl(void* renderer, void* texture);

// Forward from input_sim.c
#include "hal/input_sim.h"

// OS includes
#include "hal/display.h"
#include "hal/sound.h"
#include "hal/input.h"
#include "hal/wifi.h"
#include "gui/window_manager.h"
#include "apps/home.h"

#define SCALE 2   // window = 640x480 (2x the 320x240 display)

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow(
        "HorizonOS Simulator  |  Arrow keys = navigate  |  Enter/Space = open  |  Esc = quit",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        320 * SCALE, 240 * SCALE,
        SDL_WINDOW_SHOWN
    );
    if (!win) { printf("SDL_CreateWindow: %s\n", SDL_GetError()); return 1; }

    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, 320, 240);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        320, 240
    );

    // Connect SDL2 to display driver
    display_sim_set_sdl(renderer, texture);

    // Boot the OS
    display_init();
    sound_init();
    input_init();
    wm_init();
    app_home_launch();

    printf("HorizonOS Simulator running — Arrow keys to navigate, Enter to open, Esc to quit\n");

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:   sim_input_push(INPUT_LEFT);   break;
                    case SDLK_RIGHT:  sim_input_push(INPUT_RIGHT);  break;
                    case SDLK_RETURN:
                    case SDLK_SPACE:  sim_input_push(INPUT_SELECT); break;
                    case SDLK_ESCAPE: running = false; break;
                    default: break;
                }
            }
        }
        app_home_tick();
        display_flush();
        SDL_Delay(16);  // ~60 fps
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
