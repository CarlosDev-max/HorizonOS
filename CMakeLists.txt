cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HorizonOS C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(src/kernel)
add_subdirectory(src/hal)
add_subdirectory(src/gui)
add_subdirectory(src/services)
add_subdirectory(src/apps)

add_executable(horizonos
    src/main.c
)

target_link_libraries(horizonos
    kernel
    hal
    gui
    services
    apps
    pico_stdlib
    pico_multicore
    hardware_spi
    hardware_pwm
    hardware_flash
    pico_cyw43_arch_lwip_threadsafe_background
)

pico_add_extra_outputs(horizonos)
pico_enable_stdio_usb(horizonos 1)
pico_enable_stdio_uart(horizonos 0)

