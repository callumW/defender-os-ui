cmake_minimum_required(VERSION 3.20)

project(DefenderOsUI
    VERSION 0.1.0
    DESCRIPTION "UI library for Defender OS — GLFW + OpenGL ES"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Global compile settings
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(BUILD_TESTS "Build unit tests" ON)

# ---------------------------------------------------------------------------
# External dependencies — fetched at configure time
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# OpenGL ES 3 and EGL — provided by the system (e.g. Mesa or vendor driver)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLESv2 REQUIRED glesv2)
pkg_check_modules(EGL    REQUIRED egl)

# FreeType — for glyph rasterisation (text rendering)
find_package(Freetype REQUIRED)

# ---------------------------------------------------------------------------
# DefenderOsUICore — Logger, EventLoop, Application (no GPU dependencies)
# ---------------------------------------------------------------------------
add_library(DefenderOsUICore STATIC
    src/logging/Logger.cpp
    src/core/EventLoop.cpp
    src/core/Application.cpp
)
target_include_directories(DefenderOsUICore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(DefenderOsUICore PUBLIC cxx_std_17)

# ---------------------------------------------------------------------------
# DefenderOsUIPlatform — Window (GLFW) + Renderer (GLES) + ApplicationBuilder
# ---------------------------------------------------------------------------
add_library(DefenderOsUIPlatform STATIC
    src/window/Window.cpp
    src/renderer/Renderer.cpp
    src/text/TextRenderer.cpp
    src/platform/ApplicationBuilder.cpp
)
target_include_directories(DefenderOsUIPlatform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${GLESv2_INCLUDE_DIRS}
    ${EGL_INCLUDE_DIRS}
)
target_link_libraries(DefenderOsUIPlatform PUBLIC
    DefenderOsUICore
    glfw
    ${GLESv2_LIBRARIES}
    ${EGL_LIBRARIES}
    Freetype::Freetype
)
target_compile_options(DefenderOsUIPlatform PRIVATE
    ${GLESv2_CFLAGS_OTHER}
    ${EGL_CFLAGS_OTHER}
)

# ---------------------------------------------------------------------------
# DefenderOsUI — application entry point
# ---------------------------------------------------------------------------
add_executable(DefenderOsUI src/main.cpp)
target_link_libraries(DefenderOsUI PRIVATE
    DefenderOsUIPlatform
    DefenderOsUICore
)

# ---------------------------------------------------------------------------
# Tests — depend only on DefenderOsUICore (no GPU required)
# ---------------------------------------------------------------------------
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
