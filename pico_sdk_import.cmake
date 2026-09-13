if(DEFINED ENV{PICO_SDK_PATH} AND (NOT PICO_SDK_PATH))
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
endif()

if(NOT PICO_SDK_PATH)
    message(FATAL_ERROR "PICO_SDK_PATH not set. Set the env var or pass -DPICO_SDK_PATH=<path>")
endif()

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

