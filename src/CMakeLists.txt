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
if(TARGET SDL2::SDL2)
    target_link_libraries(defender_ui
        PUBLIC
            SDL2::SDL2
            SDL2_ttf::SDL2_ttf
            nlohmann_json::nlohmann_json
    )
else()
    # Fallback for pkg-config based SDL2
    target_link_libraries(defender_ui
        PUBLIC
            ${SDL2_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            nlohmann_json::nlohmann_json
    )
    target_include_directories(defender_ui
        PUBLIC
            ${SDL2_INCLUDE_DIRS}
            ${SDL2_TTF_INCLUDE_DIRS}
    )
endif()

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
