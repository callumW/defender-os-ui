#include "defender/theme.h"
#include "defender/logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

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

    // Read entire file into string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    // Simple JSON parser for theme configuration
    try {
        size_t pos = 0;
        
        // Parse colors section
        pos = content.find("\"colors\"");
        if (pos != std::string::npos) {
            size_t colorsStart = content.find("{", pos);
            if (colorsStart == std::string::npos) {
                throw std::runtime_error("Missing opening brace for colors section");
            }
            
            size_t colorsEnd = findMatchingBrace(content, colorsStart);
            if (colorsEnd == std::string::npos) {
                throw std::runtime_error("Missing closing brace for colors section");
            }
            
            std::string colorsSection = content.substr(colorsStart + 1, colorsEnd - colorsStart - 1);
            
            size_t colorPos = 0;
            while ((colorPos = colorsSection.find("\"", colorPos)) != std::string::npos) {
                size_t nameStart = colorPos + 1;
                size_t nameEnd = colorsSection.find("\"", nameStart);
                if (nameEnd == std::string::npos) {
                    Logger::getInstance().warning("Unclosed quote in color name, skipping");
                    break;
                }
                
                std::string colorName = colorsSection.substr(nameStart, nameEnd - nameStart);
                
                size_t arrayStart = colorsSection.find("[", nameEnd);
                size_t arrayEnd = colorsSection.find("]", arrayStart);
                
                if (arrayStart == std::string::npos || arrayEnd == std::string::npos) {
                    Logger::getInstance().warning("Missing brackets for color: " + colorName + ", skipping");
                    colorPos = nameEnd + 1;
                    continue;
                }
                
                std::string arrayStr = colorsSection.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
                
                // Parse color values
                std::istringstream iss(arrayStr);
                int r, g, b, a;
                char comma;
                iss >> r >> comma >> g >> comma >> b >> comma >> a;
                
                if (iss.fail()) {
                    Logger::getInstance().warning("Failed to parse color values for: " + colorName + ", skipping");
                    colorPos = arrayEnd + 1;
                    continue;
                }
                
                // Validate color component ranges
                if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
                    Logger::getInstance().warning("Color values out of range (0-255) for: " + colorName + ", skipping");
                    colorPos = arrayEnd + 1;
                    continue;
                }
                
                setColor(colorName, Color(static_cast<uint8_t>(r), 
                                         static_cast<uint8_t>(g), 
                                         static_cast<uint8_t>(b), 
                                         static_cast<uint8_t>(a)));
                
                Logger::getInstance().debug("Loaded color: " + colorName);
                colorPos = arrayEnd + 1;
            }
        }
        
        // Parse fonts section
        pos = content.find("\"fonts\"");
        if (pos != std::string::npos) {
            size_t fontsStart = content.find("{", pos);
            if (fontsStart == std::string::npos) {
                Logger::getInstance().warning("Missing opening brace for fonts section");
                return true; // Colors might have loaded, so return true
            }
            
            size_t fontsEnd = findMatchingBrace(content, fontsStart);
            if (fontsEnd == std::string::npos) {
                Logger::getInstance().warning("Missing closing brace for fonts section");
                return true;
            }
            
            std::string fontsSection = content.substr(fontsStart + 1, fontsEnd - fontsStart - 1);
            
            size_t fontPos = 0;
            while ((fontPos = fontsSection.find("\"", fontPos)) != std::string::npos) {
                size_t nameStart = fontPos + 1;
                size_t nameEnd = fontsSection.find("\"", nameStart);
                if (nameEnd == std::string::npos) {
                    Logger::getInstance().warning("Unclosed quote in font name");
                    break;
                }
                
                std::string fontName = fontsSection.substr(nameStart, nameEnd - nameStart);
                
                // Skip if this is a property key (path or size)
                if (fontName == "path" || fontName == "size") {
                    fontPos = nameEnd + 1;
                    continue;
                }
                
                size_t fontObjStart = fontsSection.find("{", nameEnd);
                size_t fontObjEnd = findMatchingBrace(fontsSection, fontObjStart);
                
                if (fontObjStart == std::string::npos || fontObjEnd == std::string::npos) {
                    Logger::getInstance().warning("Missing braces for font: " + fontName + ", skipping");
                    fontPos = nameEnd + 1;
                    continue;
                }
                
                std::string fontObj = fontsSection.substr(fontObjStart, fontObjEnd - fontObjStart + 1);
                
                // Parse path
                size_t pathPos = fontObj.find("\"path\"");
                if (pathPos != std::string::npos) {
                    size_t pathStart = fontObj.find("\"", pathPos + 6);
                    if (pathStart != std::string::npos) {
                        pathStart = fontObj.find("\"", pathStart + 1);
                        if (pathStart != std::string::npos) {
                            size_t pathEnd = fontObj.find("\"", pathStart + 1);
                            if (pathEnd != std::string::npos) {
                                std::string path = fontObj.substr(pathStart + 1, pathEnd - pathStart - 1);
                                setFontPath(fontName, path);
                            }
                        }
                    }
                }
                
                // Parse size
                size_t sizePos = fontObj.find("\"size\"");
                if (sizePos != std::string::npos) {
                    size_t sizeStart = fontObj.find(":", sizePos);
                    if (sizeStart != std::string::npos) {
                        size_t sizeEnd = fontObj.find_first_of(",}", sizeStart);
                        if (sizeEnd != std::string::npos) {
                            std::string sizeStr = fontObj.substr(sizeStart + 1, sizeEnd - sizeStart - 1);
                            try {
                                int size = std::stoi(trim(sizeStr));
                                setFontSize(fontName, size);
                            } catch (const std::exception& e) {
                                Logger::getInstance().warning("Failed to parse font size for: " + fontName);
                            }
                        }
                    }
                }
                        }
                    }
                }
                
                Logger::getInstance().debug("Loaded font: " + fontName);
                fontPos = fontObjEnd + 1;
            }
        }
        
        Logger::getInstance().info("Successfully loaded theme from: " + filename);
        return true;
        
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to parse theme file: " + std::string(e.what()));
        return false;
    }
}

// Helper function to find matching closing brace
// Note: This assumes braces inside JSON strings have been handled or that
// the JSON is well-formed without braces in string values
size_t Theme::findMatchingBrace(const std::string& str, size_t start) const {
    if (start >= str.length() || str[start] != '{') {
        return std::string::npos;
    }
    
    int depth = 1;
    bool inString = false;
    
    for (size_t i = start + 1; i < str.length(); ++i) {
        // Handle escape sequences
        if (str[i] == '\\' && i + 1 < str.length()) {
            ++i; // Skip next character
            continue;
        }
        
        // Track whether we're inside a string
        if (str[i] == '"') {
            inString = !inString;
            continue;
        }
        
        // Only count braces outside of strings
        if (!inString) {
            if (str[i] == '{') {
                depth++;
            } else if (str[i] == '}') {
                depth--;
                if (depth == 0) return i;
            }
        }
    }
    return std::string::npos;
}

// Helper function to trim whitespace
std::string Theme::trim(const std::string& str) const {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    if (end == std::string::npos) return "";
    
    return str.substr(start, end - start + 1);
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
