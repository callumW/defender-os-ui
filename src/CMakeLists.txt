# Source files
set(DEFENDER_UI_SOURCES
    logger.cpp
    input_state.cpp
    theme.cpp
    ui_element.cpp
    sdl_renderer.cpp
    main_window.cpp
    button.cpp
    panel.cpp
)

# Create the library
add_library(defender_ui STATIC ${DEFENDER_UI_SOURCES})

# Link SDL2 libraries
target_link_libraries(defender_ui
    PUBLIC
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
)

# Include directories
target_include_directories(defender_ui
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Set properties
set_target_properties(defender_ui PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
