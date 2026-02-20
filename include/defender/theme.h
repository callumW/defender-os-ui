#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <map>

namespace defender {

/**
 * @brief Represents a color with RGBA components
 */
struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    Color() = default;
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
};

/**
 * @brief Theme configuration for the UI system
 * 
 * Manages color schemes, fonts, and other visual configurations
 * that can be loaded from a configuration file.
 */
class Theme {
public:
    Theme();
    ~Theme() = default;

    /**
     * @brief Load theme from a JSON configuration file
     * @param filename Path to the configuration file
     * @return true if successful, false otherwise
     */
    bool loadFromFile(const std::string& filename);

    /**
     * @brief Get a color by name
     * @param name Name of the color (e.g., "background", "button", "text")
     * @return The color value
     */
    Color getColor(const std::string& name) const;

    /**
     * @brief Set a color by name
     * @param name Name of the color
     * @param color The color value
     */
    void setColor(const std::string& name, const Color& color);

    /**
     * @brief Get font path by name
     * @param name Font identifier (e.g., "default", "title", "button")
     * @return Path to the font file
     */
    std::string getFontPath(const std::string& name) const;

    /**
     * @brief Set font path by name
     * @param name Font identifier
     * @param path Path to the font file
     */
    void setFontPath(const std::string& name, const std::string& path);

    /**
     * @brief Get font size by name
     * @param name Font identifier
     * @return Font size in points
     */
    int getFontSize(const std::string& name) const;

    /**
     * @brief Set font size by name
     * @param name Font identifier
     * @param size Font size in points
     */
    void setFontSize(const std::string& name, int size);

    /**
     * @brief Get singleton instance
     */
    static Theme& getInstance();

    // Delete copy constructor and assignment operator
    Theme(const Theme&) = delete;
    Theme& operator=(const Theme&) = delete;

private:
    void setDefaults();

    std::map<std::string, Color> colors_;
    std::map<std::string, std::string> fontPaths_;
    std::map<std::string, int> fontSizes_;
};

} // namespace defender
