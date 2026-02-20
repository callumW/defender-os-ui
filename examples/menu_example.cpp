#include "defender/main_window.h"
#include "defender/button.h"
#include "defender/panel.h"
#include "defender/logger.h"
#include "defender/sdl_renderer.h"
#include "defender/theme.h"
#include <iostream>
#include <chrono>
#include <memory>

using namespace defender;

// Represents different subsystem screens
enum class Screen {
    MAIN_MENU,
    NAVIGATION,
    MEDIA,
    SETTINGS
};

class MenuSystem {
public:
    MenuSystem(MainWindow& window) : window_(window), currentScreen_(Screen::MAIN_MENU) {
        createMainMenu();
        createNavigationScreen();
        createMediaScreen();
        createSettingsScreen();
    }

    void switchToScreen(Screen screen) {
        Logger::getInstance().info("Switching to screen: " + std::to_string(static_cast<int>(screen)));
        currentScreen_ = screen;

        // Deactivate all screens
        mainMenuPanel_->setActive(false);
        navigationPanel_->setActive(false);
        mediaPanel_->setActive(false);
        settingsPanel_->setActive(false);

        // Activate the selected screen
        switch (screen) {
            case Screen::MAIN_MENU:
                mainMenuPanel_->setActive(true);
                break;
            case Screen::NAVIGATION:
                navigationPanel_->setActive(true);
                break;
            case Screen::MEDIA:
                mediaPanel_->setActive(true);
                break;
            case Screen::SETTINGS:
                settingsPanel_->setActive(true);
                break;
        }
    }

private:
    void createMainMenu() {
        auto panel = std::make_unique<Panel>();
        panel->setName("MainMenu");
        panel->setPosition(100, 100);
        panel->setSize(600, 400);

        auto navButton = std::make_unique<Button>("Navigation");
        navButton->setPosition(50, 50);
        navButton->setSize(200, 60);
        navButton->setOnClick([this]() {
            switchToScreen(Screen::NAVIGATION);
        });

        auto mediaButton = std::make_unique<Button>("Media");
        mediaButton->setPosition(50, 130);
        mediaButton->setSize(200, 60);
        mediaButton->setOnClick([this]() {
            switchToScreen(Screen::MEDIA);
        });

        auto settingsButton = std::make_unique<Button>("Settings");
        settingsButton->setPosition(50, 210);
        settingsButton->setSize(200, 60);
        settingsButton->setOnClick([this]() {
            switchToScreen(Screen::SETTINGS);
        });

        auto exitButton = std::make_unique<Button>("Exit");
        exitButton->setPosition(50, 290);
        exitButton->setSize(200, 60);
        exitButton->setOnClick([this]() {
            window_.close();
        });

        panel->addChild(std::move(navButton));
        panel->addChild(std::move(mediaButton));
        panel->addChild(std::move(settingsButton));
        panel->addChild(std::move(exitButton));

        mainMenuPanel_ = panel.get();
        window_.addChild(std::move(panel));
    }

    void createNavigationScreen() {
        auto panel = std::make_unique<Panel>();
        panel->setName("NavigationScreen");
        panel->setPosition(100, 100);
        panel->setSize(600, 400);
        panel->setActive(false); // Start inactive

        auto backButton = std::make_unique<Button>("Back to Menu");
        backButton->setPosition(50, 320);
        backButton->setSize(200, 60);
        backButton->setOnClick([this]() {
            switchToScreen(Screen::MAIN_MENU);
        });

        panel->addChild(std::move(backButton));

        navigationPanel_ = panel.get();
        window_.addChild(std::move(panel));
    }

    void createMediaScreen() {
        auto panel = std::make_unique<Panel>();
        panel->setName("MediaScreen");
        panel->setPosition(100, 100);
        panel->setSize(600, 400);
        panel->setActive(false); // Start inactive

        auto backButton = std::make_unique<Button>("Back to Menu");
        backButton->setPosition(50, 320);
        backButton->setSize(200, 60);
        backButton->setOnClick([this]() {
            switchToScreen(Screen::MAIN_MENU);
        });

        panel->addChild(std::move(backButton));

        mediaPanel_ = panel.get();
        window_.addChild(std::move(panel));
    }

    void createSettingsScreen() {
        auto panel = std::make_unique<Panel>();
        panel->setName("SettingsScreen");
        panel->setPosition(100, 100);
        panel->setSize(600, 400);
        panel->setActive(false); // Start inactive

        auto backButton = std::make_unique<Button>("Back to Menu");
        backButton->setPosition(50, 320);
        backButton->setSize(200, 60);
        backButton->setOnClick([this]() {
            switchToScreen(Screen::MAIN_MENU);
        });

        panel->addChild(std::move(backButton));

        settingsPanel_ = panel.get();
        window_.addChild(std::move(panel));
    }

    MainWindow& window_;
    Screen currentScreen_;
    
    // Raw pointers to panels (owned by window)
    Panel* mainMenuPanel_;
    Panel* navigationPanel_;
    Panel* mediaPanel_;
    Panel* settingsPanel_;
};

int main() {
    try {
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        Logger::getInstance().info("Starting Menu Example");

        // Load custom theme (optional)
        Theme::getInstance().loadFromFile("../config/theme.json");

        SDLContext sdlContext;
        if (!sdlContext.isInitialized()) {
            Logger::getInstance().error("Failed to initialize SDL");
            return 1;
        }

        MainWindow mainWindow("DefenderOS UI - Menu Example", 800, 600);
        
        // Create the menu system
        MenuSystem menuSystem(mainWindow);

        // Main loop
        InputState input;
        auto lastTime = std::chrono::high_resolution_clock::now();
        
        Logger::getInstance().info("Entering main loop");

        while (mainWindow.isOpen()) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            if (!mainWindow.processEvents(input)) {
                break;
            }

            if (input.getKeyboard().escapePressed) {
                menuSystem.switchToScreen(Screen::MAIN_MENU);
            }

            mainWindow.update(input, deltaTime);
            mainWindow.render(*mainWindow.getRenderer());
        }

        Logger::getInstance().info("Menu example completed");
        return 0;

    } catch (const std::exception& e) {
        Logger::getInstance().error(std::string("Exception: ") + e.what());
        return 1;
    }
}
