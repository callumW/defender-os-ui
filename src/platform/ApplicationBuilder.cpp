#include "ApplicationBuilder.hpp"

#include "../renderer/Renderer.hpp"
#include "../window/Window.hpp"

namespace DefenderOS {

std::unique_ptr<Application> buildApplication(const ApplicationConfig& config) {
    WindowConfig windowConfig;
    windowConfig.title = config.title;
    windowConfig.fullscreen = config.fullscreen;
    windowConfig.width = config.windowWidth;
    windowConfig.height = config.windowHeight;

    auto window = std::make_shared<Window>(windowConfig);
    auto renderer = std::make_shared<Renderer>();

    return std::make_unique<Application>(std::move(window), std::move(renderer));
}

} // namespace DefenderOS
