add_library(gui
    window_manager.c
    widgets.c
    theme.c
)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gui hal)

