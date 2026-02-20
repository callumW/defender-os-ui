#include "defender/theme.h"
#include "defender/logger.h"
#include <fstream>

namespace defender {

Theme::Theme() {
    setDefaults();
}

Theme& Theme::getInstance() {
    static Theme instance;
    return instance;
}

void Theme::setDefaults() {
    // Default colors
    colors_["background"] = Color(30, 30, 30, 255);
    colors_["foreground"] = Color(255, 255, 255, 255);
    colors_["button"] = Color(70, 130, 180, 255);
    colors_["buttonHover"] = Color(100, 149, 237, 255);
    colors_["buttonActive"] = Color(50, 110, 160, 255);
    colors_["text"] = Color(255, 255, 255, 255);
    colors_["border"] = Color(128, 128, 128, 255);
    colors_["panel"] = Color(50, 50, 50, 255);

    // Default font settings
    fontPaths_["default"] = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    fontPaths_["title"] = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf";
    fontPaths_["button"] = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";

    fontSizes_["default"] = 16;
    fontSizes_["title"] = 24;
    fontSizes_["button"] = 18;
}

bool Theme::loadFromFile(const std::string& filename) {
    Logger::getInstance().info("Loading theme from file: " + filename);
    
    // TODO: Implement JSON parsing when nlohmann/json is available
    // For now, just use defaults
    Logger::getInstance().warning("JSON parsing not yet implemented, using default theme");
    
    return true;
}

Color Theme::getColor(const std::string& name) const {
    auto it = colors_.find(name);
    if (it != colors_.end()) {
        return it->second;
    }
    Logger::getInstance().warning("Color not found: " + name + ", using white");
    return Color(255, 255, 255, 255);
}

void Theme::setColor(const std::string& name, const Color& color) {
    colors_[name] = color;
}

std::string Theme::getFontPath(const std::string& name) const {
    auto it = fontPaths_.find(name);
    if (it != fontPaths_.end()) {
        return it->second;
    }
    Logger::getInstance().warning("Font path not found: " + name + ", using default");
    return fontPaths_.at("default");
}

void Theme::setFontPath(const std::string& name, const std::string& path) {
    fontPaths_[name] = path;
}

int Theme::getFontSize(const std::string& name) const {
    auto it = fontSizes_.find(name);
    if (it != fontSizes_.end()) {
        return it->second;
    }
    Logger::getInstance().warning("Font size not found: " + name + ", using default");
    return fontSizes_.at("default");
}

void Theme::setFontSize(const std::string& name, int size) {
    fontSizes_[name] = size;
}

} // namespace defender
