cmake_minimum_required(VERSION 3.15)
project(DefenderOS_UI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build example applications" ON)

# Find required packages
find_package(SDL2 QUIET)
find_package(SDL2_ttf QUIET)

# Try to find nlohmann/json
find_package(nlohmann_json QUIET)

# Check if SDL2 was found
if(NOT SDL2_FOUND)
    message(WARNING "SDL2 not found. Please install SDL2 to build this project.")
    message(WARNING "Ubuntu/Debian: sudo apt-get install libsdl2-dev libsdl2-ttf-dev")
    message(WARNING "Fedora/RHEL: sudo dnf install SDL2-devel SDL2_ttf-devel")
    
    # Try to find using pkg-config as fallback
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL2 QUIET sdl2)
        pkg_check_modules(SDL2_TTF QUIET SDL2_ttf)
    endif()
    
    if(NOT SDL2_FOUND)
        message(FATAL_ERROR "SDL2 is required but not found. Please install it.")
    endif()
endif()

# Include directories
if(SDL2_INCLUDE_DIRS)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()
if(SDL2_TTF_INCLUDE_DIRS)
    include_directories(${SDL2_TTF_INCLUDE_DIRS})
endif()

# If nlohmann/json not found, try to use FetchContent to download it
if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Add subdirectories
add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
