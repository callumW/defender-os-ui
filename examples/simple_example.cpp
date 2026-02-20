#include "defender/main_window.h"
#include "defender/button.h"
#include "defender/panel.h"
#include "defender/logger.h"
#include "defender/sdl_renderer.h"
#include <iostream>
#include <chrono>

using namespace defender;

int main() {
    try {
        // Set log level
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        Logger::getInstance().info("Starting Simple Example");

        // Initialize SDL
        SDLContext sdlContext;
        if (!sdlContext.isInitialized()) {
            Logger::getInstance().error("Failed to initialize SDL");
            return 1;
        }

        // Create main window (root of UI tree)
        MainWindow mainWindow("DefenderOS UI - Simple Example", 800, 600);
        
        // Create a panel
        auto panel = std::make_unique<Panel>();
        panel->setName("MainPanel");
        panel->setPosition(50, 50);
        panel->setSize(700, 500);

        // Create buttons
        auto button1 = std::make_unique<Button>("Button 1");
        button1->setPosition(20, 20);
        button1->setOnClick([]() {
            Logger::getInstance().info("Button 1 clicked!");
        });

        auto button2 = std::make_unique<Button>("Button 2");
        button2->setPosition(20, 80);
        button2->setOnClick([]() {
            Logger::getInstance().info("Button 2 clicked!");
        });

        auto button3 = std::make_unique<Button>("Exit");
        button3->setPosition(20, 140);
        button3->setOnClick([&mainWindow]() {
            Logger::getInstance().info("Exit button clicked!");
            mainWindow.close();
        });

        // Build UI tree
        panel->addChild(std::move(button1));
        panel->addChild(std::move(button2));
        panel->addChild(std::move(button3));
        mainWindow.addChild(std::move(panel));

        // Main loop
        InputState input;
        auto lastTime = std::chrono::high_resolution_clock::now();
        
        Logger::getInstance().info("Entering main loop");

        while (mainWindow.isOpen()) {
            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            // Process events
            if (!mainWindow.processEvents(input)) {
                break;
            }

            // Handle escape key
            if (input.getKeyboard().escapePressed) {
                Logger::getInstance().info("Escape pressed, exiting");
                mainWindow.close();
            }

            // Update
            mainWindow.update(input, deltaTime);

            // Render
            mainWindow.render(*mainWindow.getRenderer());
        }

        Logger::getInstance().info("Example completed successfully");
        return 0;

    } catch (const std::exception& e) {
        Logger::getInstance().error(std::string("Exception: ") + e.what());
        return 1;
    }
}
