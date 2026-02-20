#pragma once

#include "defender/ui_element.h"
#include "defender/sdl_renderer.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>

namespace defender {

/**
 * @brief Main window and root of the UI tree
 * 
 * This is the root element that contains all other UI elements.
 * It manages the SDL window and provides the main update/render loop.
 */
class MainWindow : public UIElement {
public:
    /**
     * @brief Construct a main window
     * @param title Window title
     * @param width Window width in pixels
     * @param height Window height in pixels
     */
    MainWindow(const std::string& title, int width, int height);
    ~MainWindow() override;

    /**
     * @brief Check if the window is open
     */
    bool isOpen() const { return isOpen_; }

    /**
     * @brief Close the window
     */
    void close();

    /**
     * @brief Process SDL events and update input state
     * @param input InputState to update
     * @return true if window should continue running
     */
    bool processEvents(InputState& input);

    /**
     * @brief Get the renderer
     */
    Renderer* getRenderer() { return renderer_.get(); }

    /**
     * @brief Render the window and all children
     */
    void render(Renderer& renderer) override;

    // Delete copy constructor and assignment operator
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

private:
    std::unique_ptr<SDL_Window, SDLWindowDeleter> window_;
    std::unique_ptr<SDLRenderer> renderer_;
    bool isOpen_;
};

} // namespace defender
