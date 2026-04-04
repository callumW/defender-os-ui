#include "logging/Logger.hpp"
#include "platform/ApplicationBuilder.hpp"

int main() {
    DefenderOS::Logger::instance().setLogLevel(DefenderOS::LogLevel::DEBUG);
    LOG_INFO("DefenderOS UI starting");

    DefenderOS::ApplicationConfig config;
    config.title = "DefenderOS UI";
    config.fullscreen = true;

    auto app = DefenderOS::buildApplication(config);

    if (!app->initialize()) {
        LOG_CRITICAL("Application initialisation failed — exiting");
        return 1;
    }

    app->run();

    LOG_INFO("DefenderOS UI exiting");
    return 0;
}
