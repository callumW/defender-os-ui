#include "defender/main_window.h"
#include "defender/logger.h"
#include "defender/theme.h"
#include <stdexcept>

namespace defender {

MainWindow::MainWindow(const std::string& title, int width, int height)
    : UIElement()
    , window_(nullptr, SDLWindowDeleter())
    , isOpen_(true) {
    
    setName("MainWindow");
    setSize(width, height);
    setPosition(0, 0);

    Logger::getInstance().info(
        "Creating main window: " + title + " (" + 
        std::to_string(width) + "x" + std::to_string(height) + ")"
    );

    SDL_Window* raw_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!raw_window) {
        Logger::getInstance().error(std::string("Failed to create window: ") + SDL_GetError());
        throw std::runtime_error("Failed to create SDL window");
    }

    window_.reset(raw_window);

    try {
        renderer_ = std::make_unique<SDLRenderer>(window_.get());
    } catch (const std::exception& e) {
        Logger::getInstance().error(std::string("Failed to create renderer: ") + e.what());
        throw;
    }

    Logger::getInstance().info("Main window created successfully");
}

MainWindow::~MainWindow() {
    Logger::getInstance().info("Destroying main window");
}

void MainWindow::close() {
    isOpen_ = false;
    Logger::getInstance().info("Main window closed");
}

bool MainWindow::processEvents(InputState& input) {
    SDL_Event event;

    // Reset per-frame input states
    input.reset();

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                close();
                return false;

            case SDL_MOUSEMOTION:
                input.getMouse().x = event.motion.x;
                input.getMouse().y = event.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    input.getMouse().leftButton = true;
                    input.getMouse().leftClicked = true;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    input.getMouse().rightButton = true;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    input.getMouse().leftButton = false;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    input.getMouse().rightButton = false;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        input.getKeyboard().escapePressed = true;
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        input.getKeyboard().enterPressed = true;
                        break;
                    case SDLK_UP:
                        input.getKeyboard().upPressed = true;
                        break;
                    case SDLK_DOWN:
                        input.getKeyboard().downPressed = true;
                        break;
                    case SDLK_LEFT:
                        input.getKeyboard().leftPressed = true;
                        break;
                    case SDLK_RIGHT:
                        input.getKeyboard().rightPressed = true;
                        break;
                }
                break;
        }
    }

    return isOpen_;
}

void MainWindow::render(Renderer& renderer) {
    // Clear with background color
    renderer.clear();

    // Render all children
    UIElement::render(renderer);

    // Present the frame
    renderer.present();
}

} // namespace defender
