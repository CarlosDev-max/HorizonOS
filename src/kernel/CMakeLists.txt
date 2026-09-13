add_library(kernel
    scheduler.c
    memory.c
    fs.c
    shell.c
)
target_include_directories(kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kernel pico_stdlib hardware_flash)

