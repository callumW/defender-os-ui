#pragma once

#include "defender/theme.h"
#include <string>

namespace defender {

/**
 * @brief Abstract renderer interface
 * 
 * Provides an abstraction over the rendering backend (SDL2).
 * UI elements use this interface to draw themselves.
 */
class Renderer {
public:
    virtual ~Renderer() = default;

    /**
     * @brief Clear the screen
     */
    virtual void clear() = 0;

    /**
     * @brief Present the rendered frame
     */
    virtual void present() = 0;

    /**
     * @brief Set the draw color
     * @param color The color to use for drawing
     */
    virtual void setDrawColor(const Color& color) = 0;

    /**
     * @brief Draw a filled rectangle
     * @param x X coordinate
     * @param y Y coordinate
     * @param width Rectangle width
     * @param height Rectangle height
     */
    virtual void fillRect(int x, int y, int width, int height) = 0;

    /**
     * @brief Draw a rectangle outline
     * @param x X coordinate
     * @param y Y coordinate
     * @param width Rectangle width
     * @param height Rectangle height
     */
    virtual void drawRect(int x, int y, int width, int height) = 0;

    /**
     * @brief Draw text
     * @param text The text to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param fontName Font identifier from theme
     * @param color Text color
     */
    virtual void drawText(const std::string& text, int x, int y, 
                         const std::string& fontName, const Color& color) = 0;

    /**
     * @brief Get text dimensions
     * @param text The text to measure
     * @param fontName Font identifier from theme
     * @param width Output width
     * @param height Output height
     */
    virtual void getTextSize(const std::string& text, const std::string& fontName,
                           int& width, int& height) = 0;
};

} // namespace defender
