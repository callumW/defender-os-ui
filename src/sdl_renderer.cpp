#include "defender/sdl_renderer.h"
#include "defender/theme.h"
#include "defender/logger.h"
#include <stdexcept>

namespace defender {

SDLContext::SDLContext() : initialized_(false) {
    Logger::getInstance().info("Initializing SDL");
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Logger::getInstance().error(std::string("SDL initialization failed: ") + SDL_GetError());
        return;
    }

    if (TTF_Init() < 0) {
        Logger::getInstance().error(std::string("SDL_ttf initialization failed: ") + TTF_GetError());
        SDL_Quit();
        return;
    }

    initialized_ = true;
    Logger::getInstance().info("SDL initialized successfully");
}

SDLContext::~SDLContext() {
    if (initialized_) {
        Logger::getInstance().info("Shutting down SDL");
        TTF_Quit();
        SDL_Quit();
    }
}

SDLRenderer::SDLRenderer(SDL_Window* window)
    : renderer_(nullptr, SDLRendererDeleter()) {
    
    if (!window) {
        throw std::runtime_error("Cannot create renderer with null window");
    }

    SDL_Renderer* raw_renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!raw_renderer) {
        Logger::getInstance().error(std::string("Failed to create renderer: ") + SDL_GetError());
        throw std::runtime_error("Failed to create SDL renderer");
    }

    renderer_.reset(raw_renderer);
    Logger::getInstance().info("SDL Renderer created successfully");
}

SDLRenderer::~SDLRenderer() {
    fonts_.clear(); // Clear fonts before renderer is destroyed
    Logger::getInstance().debug("SDL Renderer destroyed");
}

void SDLRenderer::clear() {
    auto bgColor = Theme::getInstance().getColor("background");
    SDL_SetRenderDrawColor(renderer_.get(), bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer_.get());
}

void SDLRenderer::present() {
    SDL_RenderPresent(renderer_.get());
}

void SDLRenderer::setDrawColor(const Color& color) {
    SDL_SetRenderDrawColor(renderer_.get(), color.r, color.g, color.b, color.a);
}

void SDLRenderer::fillRect(int x, int y, int width, int height) {
    SDL_Rect rect = { x, y, width, height };
    SDL_RenderFillRect(renderer_.get(), &rect);
}

void SDLRenderer::drawRect(int x, int y, int width, int height) {
    SDL_Rect rect = { x, y, width, height };
    SDL_RenderDrawRect(renderer_.get(), &rect);
}

void SDLRenderer::drawText(const std::string& text, int x, int y,
                           const std::string& fontName, const Color& color) {
    if (text.empty()) {
        return;
    }

    TTF_Font* font = getFont(fontName);
    if (!font) {
        Logger::getInstance().warning("Failed to load font for text rendering");
        return;
    }

    SDL_Color sdlColor = { color.r, color.g, color.b, color.a };
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), sdlColor);
    
    if (!surface) {
        Logger::getInstance().warning(std::string("Failed to render text: ") + TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_.get(), surface);
    
    if (texture) {
        SDL_Rect destRect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer_.get(), texture, nullptr, &destRect);
        SDL_DestroyTexture(texture);
    }

    SDL_FreeSurface(surface);
}

void SDLRenderer::getTextSize(const std::string& text, const std::string& fontName,
                              int& width, int& height) {
    width = 0;
    height = 0;

    if (text.empty()) {
        return;
    }

    TTF_Font* font = getFont(fontName);
    if (!font) {
        return;
    }

    TTF_SizeText(font, text.c_str(), &width, &height);
}

TTF_Font* SDLRenderer::getFont(const std::string& fontName) {
    // Check if font is already loaded
    auto it = fonts_.find(fontName);
    if (it != fonts_.end()) {
        return it->second.get();
    }

    // Load font from theme
    auto& theme = Theme::getInstance();
    std::string fontPath = theme.getFontPath(fontName);
    int fontSize = theme.getFontSize(fontName);

    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
    
    if (!font) {
        Logger::getInstance().error(
            "Failed to load font " + fontPath + ": " + TTF_GetError()
        );
        return nullptr;
    }

    fonts_[fontName] = std::unique_ptr<TTF_Font, TTFFontDeleter>(font, TTFFontDeleter());
    Logger::getInstance().debug("Loaded font: " + fontName);

    return font;
}

} // namespace defender
