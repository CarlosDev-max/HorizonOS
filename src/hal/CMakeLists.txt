add_library(hal
    display.c
    touch.c
    sound.c
    wifi.c
    input.c
)
target_include_directories(hal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hal
    pico_stdlib
    hardware_spi
    hardware_pwm
    hardware_gpio
    pico_cyw43_arch_lwip_threadsafe_background
)
