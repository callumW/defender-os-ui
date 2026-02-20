# Test for logger
add_executable(test_logger test_logger.cpp)
target_link_libraries(test_logger defender_ui)
add_test(NAME TestLogger COMMAND test_logger)

# Test for UI element tree
add_executable(test_ui_element test_ui_element.cpp)
target_link_libraries(test_ui_element defender_ui)
add_test(NAME TestUIElement COMMAND test_ui_element)

# Test for theme
add_executable(test_theme test_theme.cpp)
target_link_libraries(test_theme defender_ui)
add_test(NAME TestTheme COMMAND test_theme)

# Test for input state
add_executable(test_input_state test_input_state.cpp)
target_link_libraries(test_input_state defender_ui)
add_test(NAME TestInputState COMMAND test_input_state)
