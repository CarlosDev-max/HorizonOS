# Resolve PICO_SDK_PATH from env var if not already set via -D
if(DEFINED ENV{PICO_SDK_PATH} AND (NOT PICO_SDK_PATH))
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
endif()

if(NOT PICO_SDK_PATH)
    message(FATAL_ERROR
        "PICO_SDK_PATH is not set.\n"
        "Pass it with: cmake -DPICO_SDK_PATH=/path/to/pico-sdk\n"
        "Or set the PICO_SDK_PATH environment variable.")
endif()

# Convert relative path to absolute (relative to the cmake build directory).
# This handles the case where -DPICO_SDK_PATH=../pico-sdk is passed from
# inside the build/ directory — cmake would otherwise resolve it relative
# to the source directory, pointing outside the repo.
get_filename_component(PICO_SDK_PATH "${PICO_SDK_PATH}"
    REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
set(PICO_SDK_PATH "${PICO_SDK_PATH}" CACHE PATH "Raspberry Pi Pico SDK path" FORCE)

message(STATUS "HorizonOS: using pico-sdk at ${PICO_SDK_PATH}")
include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")
