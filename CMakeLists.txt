cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
include(FetchContent)

project(HorizonOS C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# ─── LittleFS ────────────────────────────────────────────────────────────────
FetchContent_Declare(
  littlefs
  GIT_REPOSITORY https://github.com/littlefs-project/littlefs.git
  GIT_TAG        v2.8.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(littlefs)

add_library(littlefs_lib STATIC
  ${littlefs_SOURCE_DIR}/lfs.c
  ${littlefs_SOURCE_DIR}/lfs_util.c
)
target_include_directories(littlefs_lib PUBLIC ${littlefs_SOURCE_DIR})
target_compile_definitions(littlefs_lib PUBLIC LFS_NO_MALLOC=0)

# ─── OS modules ──────────────────────────────────────────────────────────────
add_subdirectory(src/kernel)
add_subdirectory(src/hal)
add_subdirectory(src/gui)
add_subdirectory(src/services)
add_subdirectory(src/apps)

# ─── Main executable ─────────────────────────────────────────────────────────
add_executable(horizonos src/main.c)

target_link_libraries(horizonos
  kernel
  hal
  gui
  services
  apps
  littlefs_lib
  pico_stdlib
  pico_multicore
  hardware_spi
  hardware_pwm
  hardware_flash
  hardware_dma
  pico_cyw43_arch_lwip_threadsafe_background
)

# USB stdio for UART shell on Wokwi / dev board
pico_enable_stdio_usb(horizonos 1)
pico_enable_stdio_uart(horizonos 0)

pico_add_extra_outputs(horizonos)
