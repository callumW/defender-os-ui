include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
# Do not override the parent project's compiler/linker settings on Windows.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# ---------------------------------------------------------------------------
# Helper to create a test executable linked against DefenderOsUICore
# ---------------------------------------------------------------------------
function(add_core_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE
        GTest::gtest_main
        DefenderOsUICore
    )
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
    gtest_discover_tests(${name})
endfunction()

add_core_test(test_logger      test_logger.cpp)
add_core_test(test_event_loop  test_event_loop.cpp)
add_core_test(test_application test_application.cpp)
