add_library(apps
    home.c
    files.c
    settings.c
    network.c
)
target_include_directories(apps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(apps gui services hal)

