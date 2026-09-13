add_library(services
    event_bus.c
    app_manager.c
)
target_include_directories(services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(services kernel hal)

