#pragma once

#include "../core/Application.hpp"

#include <memory>
#include <string>

namespace DefenderOS {

struct ApplicationConfig {
    std::string title{"DefenderOS UI"};
    bool fullscreen{true};
    int windowWidth{1920};
    int windowHeight{1080};
};

/// Factory that constructs a production Application backed by a real GLFW
/// Window and an OpenGL ES Renderer.
///
/// This is the only file in the platform layer that ties together the concrete
/// implementations.  Tests bypass this builder entirely, injecting mocks
/// directly into Application.
[[nodiscard]] std::unique_ptr<Application>
buildApplication(const ApplicationConfig& config = {});

} // namespace DefenderOS
