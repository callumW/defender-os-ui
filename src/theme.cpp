#include "defender/theme.h"
#include "defender/logger.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

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
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::getInstance().error("Failed to open theme file: " + filename);
        return false;
    }

    try {
        // Parse JSON file using nlohmann/json
        json j;
        file >> j;
        file.close();
        
        // Parse colors section
        if (j.contains("colors") && j["colors"].is_object()) {
            for (auto& [colorName, colorArray] : j["colors"].items()) {
                if (colorArray.is_array() && colorArray.size() >= 4) {
                    try {
                        int r = colorArray[0].get<int>();
                        int g = colorArray[1].get<int>();
                        int b = colorArray[2].get<int>();
                        int a = colorArray[3].get<int>();
                        
                        // Validate color component ranges
                        if (r < 0 || r > 255 || g < 0 || g > 255 || 
                            b < 0 || b > 255 || a < 0 || a > 255) {
                            Logger::getInstance().warning(
                                "Color values out of range (0-255) for: " + colorName + ", skipping"
                            );
                            continue;
                        }
                        
                        setColor(colorName, Color(
                            static_cast<uint8_t>(r),
                            static_cast<uint8_t>(g),
                            static_cast<uint8_t>(b),
                            static_cast<uint8_t>(a)
                        ));
                        
                        Logger::getInstance().debug("Loaded color: " + colorName);
                    } catch (const json::exception& e) {
                        Logger::getInstance().warning(
                            "Failed to parse color: " + colorName + " - " + e.what()
                        );
                    }
                } else {
                    Logger::getInstance().warning(
                        "Invalid color format for: " + colorName + ", expected array of 4 integers"
                    );
                }
            }
        }
        
        // Parse fonts section
        if (j.contains("fonts") && j["fonts"].is_object()) {
            for (auto& [fontName, fontObj] : j["fonts"].items()) {
                if (fontObj.is_object()) {
                    try {
                        // Parse font path
                        if (fontObj.contains("path") && fontObj["path"].is_string()) {
                            std::string path = fontObj["path"].get<std::string>();
                            setFontPath(fontName, path);
                        }
                        
                        // Parse font size
                        if (fontObj.contains("size") && fontObj["size"].is_number_integer()) {
                            int size = fontObj["size"].get<int>();
                            setFontSize(fontName, size);
                        }
                        
                        Logger::getInstance().debug("Loaded font: " + fontName);
                    } catch (const json::exception& e) {
                        Logger::getInstance().warning(
                            "Failed to parse font: " + fontName + " - " + e.what()
                        );
                    }
                } else {
                    Logger::getInstance().warning(
                        "Invalid font format for: " + fontName + ", expected object with path and size"
                    );
                }
            }
        }
        
        Logger::getInstance().info("Successfully loaded theme from: " + filename);
        return true;
        
    } catch (const json::exception& e) {
        Logger::getInstance().error("Failed to parse JSON file: " + std::string(e.what()));
        return false;
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to load theme file: " + std::string(e.what()));
        return false;
    }
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
