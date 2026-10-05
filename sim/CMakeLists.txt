cmake_minimum_required(VERSION 3.13)
project(HorizonOS_Sim C)
set(CMAKE_C_STANDARD 11)

# ── SDL2 ──────────────────────────────────────────────────────────────────────
# On MSYS2/MINGW64 or Linux with libsdl2-dev: use find_package
# For cross-compile CI with explicit paths, SDL2_INCLUDE_DIR/SDL2_LIBRARY can be set via -D
if(DEFINED SDL2_INCLUDE_DIR AND DEFINED SDL2_LIBRARY)
    include_directories(${SDL2_INCLUDE_DIR})
    set(SDL2_LIBS ${SDL2_LIBRARY})
    if(DEFINED SDL2_MAIN_LIBRARY)
        list(PREPEND SDL2_LIBS ${SDL2_MAIN_LIBRARY})
    endif()
else()
    find_package(SDL2 REQUIRED)
    if(TARGET SDL2::SDL2)
        # Modern CMake SDL2 targets (MSYS2 SDL2 2.x)
        set(SDL2_LIBS SDL2::SDL2main SDL2::SDL2)
    else()
        include_directories(${SDL2_INCLUDE_DIRS})
        set(SDL2_LIBS ${SDL2_LIBRARIES})
    endif()
endif()

# ── Include paths ─────────────────────────────────────────────────────────────
include_directories(
    ${CMAKE_SOURCE_DIR}/../src
    ${CMAKE_SOURCE_DIR}/../src/hal
    ${CMAKE_SOURCE_DIR}/../src/gui
    ${CMAKE_SOURCE_DIR}/../src/kernel
    ${CMAKE_SOURCE_DIR}/../src/services
    ${CMAKE_SOURCE_DIR}/../src/apps
    ${CMAKE_SOURCE_DIR}/stubs
    ${CMAKE_SOURCE_DIR}/hal
)

# ── Sources ───────────────────────────────────────────────────────────────────
set(KERNEL_SRC
    ../src/kernel/scheduler.c
    ../src/kernel/memory.c
    ../src/kernel/shell.c
    ../src/kernel/fs.c
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
    ${KERNEL_SRC} ${GUI_SRC} ${SERVICES_SRC} ${APPS_SRC} ${SIM_SRC}
)

target_link_libraries(horizonos_sim ${SDL2_LIBS})

if(WIN32)
    set_target_properties(horizonos_sim PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
