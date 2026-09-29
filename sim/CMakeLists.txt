cmake_minimum_required(VERSION 3.13)
project(HorizonOS_Sim C)
set(CMAKE_C_STANDARD 11)

# ── SDL2 ──────────────────────────────────────────────────────────────────────
if(DEFINED SDL2_INCLUDE_DIR AND DEFINED SDL2_LIBRARY)
    # Provided explicitly (cross-compile from CI)
    include_directories(${SDL2_INCLUDE_DIR})
    set(SDL2_LIBS ${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY})
else()
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    set(SDL2_LIBS ${SDL2_LIBRARIES})
endif()

# ── Include paths ─────────────────────────────────────────────────────────────
include_directories(
    ${CMAKE_SOURCE_DIR}/../src          # "hal/display.h", "gui/...", "kernel/..."
    ${CMAKE_SOURCE_DIR}/../src/hal      # "display.h", "input.h", "sound.h" ...
    ${CMAKE_SOURCE_DIR}/../src/gui      # "window_manager.h", "widgets.h", "theme.h"
    ${CMAKE_SOURCE_DIR}/../src/kernel   # "scheduler.h", "memory.h" ...
    ${CMAKE_SOURCE_DIR}/../src/services # "event_bus.h", "app_manager.h"
    ${CMAKE_SOURCE_DIR}/../src/apps     # "home.h", "files.h" ...
    ${CMAKE_SOURCE_DIR}/stubs           # "pico/stdlib.h", "hardware/gpio.h", "lfs.h"
    ${CMAKE_SOURCE_DIR}/hal             # "input_sim.h"
)

# ── Source files ──────────────────────────────────────────────────────────────
# OS kernel (skip fs.c — needs LittleFS, not required for sim)
set(KERNEL_SRC
    ../src/kernel/scheduler.c
    ../src/kernel/memory.c
    ../src/kernel/shell.c
    ../src/kernel/fs.c          # compiled with stub lfs.h — just won't mount
)
set(GUI_SRC
    ../src/gui/window_manager.c
    ../src/gui/widgets.c
)
set(SERVICES_SRC
    ../src/services/event_bus.c
    ../src/services/app_manager.c
)
set(APPS_SRC
    ../src/apps/home.c
    ../src/apps/files.c
    ../src/apps/settings.c
    ../src/apps/network.c
)
set(SIM_SRC
    main_sim.c
    sim_stubs.c
    hal/display_sim.c
    hal/input_sim.c
    hal/sound_sim.c
    hal/touch_sim.c
    hal/wifi_sim.c
)

add_executable(horizonos_sim
    ${KERNEL_SRC}
    ${GUI_SRC}
    ${SERVICES_SRC}
    ${APPS_SRC}
    ${SIM_SRC}
)

target_link_libraries(horizonos_sim ${SDL2_LIBS})

# Windows: suppress console window (show only the SDL2 window)
if(WIN32)
    set_target_properties(horizonos_sim PROPERTIES WIN32_EXECUTABLE TRUE)
    target_link_libraries(horizonos_sim mingw32)
endif()
