#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "kernel/scheduler.h"
#include "kernel/shell.h"
#include "hal/display.h"
#include "hal/sound.h"
#include "hal/input.h"
#include "hal/wifi.h"
#include "gui/window_manager.h"
#include "apps/home.h"

// ── Core 1: WiFi stack ────────────────────────────────────────────────────────
static void core1_entry(void) {
    wifi_init();
    while (1) wifi_poll();
}

// ── Scheduler tasks ───────────────────────────────────────────────────────────
static void task_home(void)  { app_home_tick(); }
static void task_shell(void) { shell_tick(); }

// ── Boot ──────────────────────────────────────────────────────────────────────
int main(void) {
    stdio_init_all();

    // Hardware
    display_init();
    sound_init();
    input_init();

    // GUI
    wm_init();

    // WiFi on core 1
    multicore_launch_core1(core1_entry);

    // Shell
    shell_init();

    // Launch home screen (first thing the user sees)
    app_home_launch();

    // Register scheduler tasks (cooperative — each yields on return)
    scheduler_add("home",  task_home);
    scheduler_add("shell", task_shell);

    // Main loop
    scheduler_run();
    return 0;
}
