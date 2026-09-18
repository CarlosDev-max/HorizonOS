# Resolve PICO_SDK_PATH (from -D flag or env var)
if(DEFINED ENV{PICO_SDK_PATH} AND NOT PICO_SDK_PATH)
    set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")
endif()

if(NOT PICO_SDK_PATH)
    message(FATAL_ERROR
        "PICO_SDK_PATH not set.\n"
        "Pass -DPICO_SDK_PATH=/path/to/pico-sdk or set the env var.")
endif()

# Resolve relative paths to absolute (relative to cmake binary dir)
get_filename_component(PICO_SDK_PATH "${PICO_SDK_PATH}"
    REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
set(PICO_SDK_PATH "${PICO_SDK_PATH}" CACHE PATH "Raspberry Pi Pico SDK" FORCE)
message(STATUS "HorizonOS: pico-sdk → ${PICO_SDK_PATH}")

include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")
