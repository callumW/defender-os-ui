# Quick Start Guide

## Installation

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake g++ libsdl2-dev libsdl2-ttf-dev

# Clone and build
git clone https://github.com/callumW/defender-os-ui.git
cd defender-os-ui
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run tests
ctest --output-on-failure

# Try examples
./examples/simple_example
./examples/menu_example
```

## Basic Usage

### 1. Initialize SDL

```cpp
#include "defender/sdl_renderer.h"

using namespace defender;

SDLContext sdl; // RAII - automatic cleanup
if (!sdl.isInitialized()) {
    return 1;
}
```

### 2. Create Main Window

```cpp
#include "defender/main_window.h"

MainWindow window("My App", 800, 600);
```

### 3. Create UI Elements

```cpp
#include "defender/button.h"
#include "defender/panel.h"

// Create a panel
auto panel = std::make_unique<Panel>();
panel->setPosition(50, 50);
panel->setSize(700, 500);

// Create a button
auto button = std::make_unique<Button>("Click Me");
button->setPosition(20, 20);
button->setOnClick([]() {
    Logger::getInstance().info("Button clicked!");
});

// Build UI tree
panel->addChild(std::move(button));
window.addChild(std::move(panel));
```

### 4. Main Loop

```cpp
#include "defender/input_state.h"
#include <chrono>

InputState input;
auto lastTime = std::chrono::high_resolution_clock::now();

while (window.isOpen()) {
    // Calculate delta time
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(
        currentTime - lastTime
    ).count();
    lastTime = currentTime;

    // Process events
    if (!window.processEvents(input)) {
        break;
    }

    // Update
    window.update(input, deltaTime);

    // Render
    window.render(*window.getRenderer());
}
```

## Common Patterns

### Create Custom Component

```cpp
class MyComponent : public UIElement {
public:
    MyComponent() : UIElement() {
        setName("MyComponent");
        setSize(100, 50);
    }

    void update(const InputState& input, float deltaTime) override {
        if (!isActive()) return;
        
        // Your update logic here
        
        UIElement::update(input, deltaTime);
    }

    void render(Renderer& renderer) override {
        if (!isActive()) return;
        
        int absX, absY;
        getAbsolutePosition(absX, absY);
        
        // Your rendering here
        renderer.setDrawColor(Color(100, 100, 100, 255));
        renderer.fillRect(absX, absY, width_, height_);
        
        UIElement::render(renderer);
    }
};
```

### Switch Between Screens

```cpp
void switchToScreen(Panel* newScreen, Panel* oldScreen) {
    oldScreen->setActive(false);  // Hides old screen and its children
    newScreen->setActive(true);   // Shows new screen and its children
}

// Usage
switchToScreen(settingsPanel, mainMenuPanel);
```

### Handle Input

```cpp
void MyElement::update(const InputState& input, float deltaTime) {
    if (!isActive()) return;
    
    // Check mouse
    if (input.getMouse().leftClicked) {
        if (containsPoint(input.getMouse().x, input.getMouse().y)) {
            // Clicked on this element
        }
    }
    
    // Check keyboard
    if (input.getKeyboard().escapePressed) {
        // Handle escape
    }
    
    UIElement::update(input, deltaTime);
}
```

### Use Theme

```cpp
#include "defender/theme.h"

// Get colors
Color bgColor = Theme::getInstance().getColor("background");
Color btnColor = Theme::getInstance().getColor("button");

// Set custom colors
Theme::getInstance().setColor("myColor", Color(255, 128, 0, 255));

// Get fonts
std::string fontPath = Theme::getInstance().getFontPath("default");
int fontSize = Theme::getInstance().getFontSize("title");
```

### Logging

```cpp
#include "defender/logger.h"

// Set log level
Logger::getInstance().setLogLevel(LogLevel::DEBUG);

// Log messages
Logger::getInstance().debug("Debug information");
Logger::getInstance().info("Operation completed");
Logger::getInstance().warning("Potential issue");
Logger::getInstance().error("Error occurred");
```

## API Reference

### UIElement

```cpp
// Position (relative to parent)
void setPosition(int x, int y);
void getPosition(int& x, int& y) const;
void getAbsolutePosition(int& x, int& y) const;

// Size
void setSize(int width, int height);
void getSize(int& width, int& height) const;

// Tree operations
void addChild(std::unique_ptr<UIElement> child);
bool removeChild(UIElement* child);
size_t getChildCount() const;
UIElement* getChild(size_t index);
UIElement* getParent();

// State
void setActive(bool active);
bool isActive() const;

// Virtual methods to override
virtual void update(const InputState& input, float deltaTime);
virtual void render(Renderer& renderer);
```

### Button

```cpp
// Text
void setText(const std::string& text);
const std::string& getText() const;

// Callback
void setOnClick(std::function<void()> callback);

// Inherited from UIElement
setPosition(x, y);
setSize(width, height);
```

### Panel

```cpp
// Drawing options
void setDrawBackground(bool draw);
void setDrawBorder(bool draw);

// Inherited from UIElement
setPosition(x, y);
setSize(width, height);
addChild(std::move(child));
```

### MainWindow

```cpp
// Window state
bool isOpen() const;
void close();

// Event processing
bool processEvents(InputState& input);

// Renderer access
Renderer* getRenderer();
```

### InputState

```cpp
// Mouse
const MouseState& getMouse() const;
int x, y;              // Position
bool leftButton;       // Currently held
bool rightButton;      // Currently held
bool leftClicked;      // Clicked this frame

// Keyboard
const KeyboardState& getKeyboard() const;
bool escapePressed;    // Pressed this frame
bool enterPressed;     // Pressed this frame
bool upPressed;        // Pressed this frame
bool downPressed;      // Pressed this frame
bool leftPressed;      // Pressed this frame
bool rightPressed;     // Pressed this frame

// Reset per-frame states
void reset();
```

### Renderer

```cpp
void clear();
void present();
void setDrawColor(const Color& color);
void fillRect(int x, int y, int width, int height);
void drawRect(int x, int y, int width, int height);
void drawText(const std::string& text, int x, int y, 
             const std::string& fontName, const Color& color);
void getTextSize(const std::string& text, const std::string& fontName,
                int& width, int& height);
```

### Theme

```cpp
// Singleton access
static Theme& getInstance();

// Colors
Color getColor(const std::string& name) const;
void setColor(const std::string& name, const Color& color);

// Fonts
std::string getFontPath(const std::string& name) const;
void setFontPath(const std::string& name, const std::string& path);
int getFontSize(const std::string& name) const;
void setFontSize(const std::string& name, int size);

// Loading
bool loadFromFile(const std::string& filename);
```

### Logger

```cpp
// Singleton access
static Logger& getInstance();

// Log level
void setLogLevel(LogLevel level);
LogLevel getLogLevel() const;

// Logging
void debug(const std::string& message);
void info(const std::string& message);
void warning(const std::string& message);
void error(const std::string& message);
```

## Tips

1. **Always check isActive()** in update() and render()
2. **Call parent update/render** to process children
3. **Use std::move()** when adding children (transfers ownership)
4. **Get absolute position** for hit testing and rendering
5. **Reset input state** each frame with `input.reset()`
6. **Use Logger** for debugging and error reporting
7. **Customize themes** via Theme::getInstance()

## More Information

- [README.md](README.md) - Overview and features
- [ARCHITECTURE.md](ARCHITECTURE.md) - Detailed design
- [BUILD.md](BUILD.md) - Build instructions
- [DEPENDENCIES.md](DEPENDENCIES.md) - Required packages
- [examples/](examples/) - Working code examples
