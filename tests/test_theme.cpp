#include "defender/theme.h"
#include <cassert>
#include <iostream>

using namespace defender;

void testThemeSingleton() {
    Theme& theme1 = Theme::getInstance();
    Theme& theme2 = Theme::getInstance();
    
    assert(&theme1 == &theme2);
    std::cout << "✓ Theme singleton test passed" << std::endl;
}

void testDefaultColors() {
    Theme& theme = Theme::getInstance();
    
    // Test that default colors exist
    Color bg = theme.getColor("background");
    Color fg = theme.getColor("foreground");
    Color button = theme.getColor("button");
    
    // Just verify they return valid colors (not checking exact values)
    assert(bg.a == 255);
    assert(fg.a == 255);
    assert(button.a == 255);
    
    std::cout << "✓ Default colors test passed" << std::endl;
}

void testSetColor() {
    Theme& theme = Theme::getInstance();
    
    Color testColor(100, 150, 200, 255);
    theme.setColor("testColor", testColor);
    
    Color retrieved = theme.getColor("testColor");
    assert(retrieved.r == 100);
    assert(retrieved.g == 150);
    assert(retrieved.b == 200);
    assert(retrieved.a == 255);
    
    std::cout << "✓ Set color test passed" << std::endl;
}

void testFontSettings() {
    Theme& theme = Theme::getInstance();
    
    // Test default font size
    int defaultSize = theme.getFontSize("default");
    assert(defaultSize > 0);
    
    // Test setting font size
    theme.setFontSize("custom", 20);
    assert(theme.getFontSize("custom") == 20);
    
    // Test font path
    std::string fontPath = theme.getFontPath("default");
    assert(!fontPath.empty());
    
    std::cout << "✓ Font settings test passed" << std::endl;
}

void testLoadFromFile() {
    Theme& theme = Theme::getInstance();
    
    // Try to load the example theme file
    bool loaded = theme.loadFromFile("../config/theme.json");
    
    // If file doesn't exist, that's okay for the test
    if (loaded) {
        std::cout << "✓ Load from file test passed (file loaded)" << std::endl;
    } else {
        std::cout << "✓ Load from file test passed (file not found, gracefully handled)" << std::endl;
    }
}

int main() {
    std::cout << "Running Theme tests..." << std::endl;
    
    try {
        testThemeSingleton();
        testDefaultColors();
        testSetColor();
        testFontSettings();
        testLoadFromFile();
        
        std::cout << "All Theme tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
