#pragma once

#include "defender/renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <map>
#include <string>

namespace defender {

/**
 * @brief Custom deleter for SDL_Window
 */
struct SDLWindowDeleter {
    void operator()(SDL_Window* window) const {
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
};

/**
 * @brief Custom deleter for SDL_Renderer
 */
struct SDLRendererDeleter {
    void operator()(SDL_Renderer* renderer) const {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
    }
};

/**
 * @brief Custom deleter for TTF_Font
 */
struct TTFFontDeleter {
    void operator()(TTF_Font* font) const {
        if (font) {
            TTF_CloseFont(font);
        }
    }
};

/**
 * @brief SDL2 implementation of the Renderer interface
 * 
 * Uses RAII wrappers (unique_ptr with custom deleters) for SDL resources.
 */
class SDLRenderer : public Renderer {
public:
    /**
     * @brief Construct SDL renderer
     * @param window SDL window pointer
     */
    explicit SDLRenderer(SDL_Window* window);
    ~SDLRenderer() override;

    void clear() override;
    void present() override;
    void setDrawColor(const Color& color) override;
    void fillRect(int x, int y, int width, int height) override;
    void drawRect(int x, int y, int width, int height) override;
    void drawText(const std::string& text, int x, int y,
                 const std::string& fontName, const Color& color) override;
    void getTextSize(const std::string& text, const std::string& fontName,
                    int& width, int& height) override;

    /**
     * @brief Get the underlying SDL_Renderer
     */
    SDL_Renderer* getSDLRenderer() { return renderer_.get(); }

private:
    TTF_Font* getFont(const std::string& fontName);

    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> renderer_;
    std::map<std::string, std::unique_ptr<TTF_Font, TTFFontDeleter>> fonts_;
};

/**
 * @brief RAII wrapper for SDL initialization
 */
class SDLContext {
public:
    SDLContext();
    ~SDLContext();

    bool isInitialized() const { return initialized_; }

    // Delete copy constructor and assignment operator
    SDLContext(const SDLContext&) = delete;
    SDLContext& operator=(const SDLContext&) = delete;

private:
    bool initialized_;
};

} // namespace defender
