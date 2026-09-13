#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "kernel/scheduler.h"
#include "hal/display.h"
#include "hal/touch.h"
#include "hal/sound.h"
#include "hal/wifi.h"
#include "gui/window_manager.h"
#include "apps/home.h"

// Core 1: WiFi stack
void core1_entry() {
    wifi_init();
    while (1) {
        wifi_poll();
    }
}

int main() {
    stdio_init_all();

    // Hardware init
    display_init();
    touch_init();
    sound_init();

    // GUI init
    wm_init();

    // Launch WiFi on core 1
    multicore_launch_core1(core1_entry);

    // Launch home app
    app_home_launch();

    // Kernel scheduler loop
    scheduler_run();

    return 0;
}

