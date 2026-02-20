# DefenderOS UI Library

A C++ UI library using SDL2 for in-car navigation computer operating systems based on buildroot.

## Features

- **Tree-based Architecture**: UI elements organized in a parent-child hierarchy
- **RAII & Smart Pointers**: Automatic resource management using modern C++ practices
- **SOLID Principles**: Clean, maintainable, and extensible design
- **Theme Support**: Configurable color schemes and fonts via JSON
- **Input Abstraction**: Unified input handling through InputState
- **Active/Inactive States**: Efficient rendering by toggling element states
- **Logging System**: Unified logging across all modules
- **Unit Tests**: Comprehensive testing with CTest

## Architecture

The UI system is based on a tree structure where:
- `MainWindow` is the root element
- Each `UIElement` can have a parent and children
- Coordinates and scaling are relative to parent
- Active/inactive states propagate down the tree
- Only active elements are updated and rendered

## Building

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install -y cmake g++ libsdl2-dev libsdl2-ttf-dev

# Fedora/RHEL
sudo dnf install cmake gcc-c++ SDL2-devel SDL2_ttf-devel
```

### Compile

```bash
mkdir build && cd build
cmake ..
make
```

### Run Tests

```bash
cd build
ctest --output-on-failure
```

### Run Examples

```bash
# Simple example with buttons
./examples/simple_example

# Menu-based example (navigation system demo)
./examples/menu_example
```

## Usage

### Basic Example

```cpp
#include "defender/main_window.h"
#include "defender/button.h"
#include "defender/sdl_renderer.h"

using namespace defender;

int main() {
    // Initialize SDL
    SDLContext sdlContext;
    
    // Create main window (root of UI tree)
    MainWindow mainWindow("My App", 800, 600);
    
    // Create a button
    auto button = std::make_unique<Button>("Click Me");
    button->setPosition(100, 100);
    button->setOnClick([]() {
        Logger::getInstance().info("Button clicked!");
    });
    
    // Add button to window
    mainWindow.addChild(std::move(button));
    
    // Main loop
    InputState input;
    while (mainWindow.isOpen()) {
        mainWindow.processEvents(input);
        mainWindow.update(input, 0.016f);
        mainWindow.render(*mainWindow.getRenderer());
    }
    
    return 0;
}
```

### Creating Custom UI Elements

```cpp
class MyElement : public UIElement {
public:
    void update(const InputState& input, float deltaTime) override {
        if (!isActive()) return;
        // Custom update logic
        UIElement::update(input, deltaTime); // Update children
    }
    
    void render(Renderer& renderer) override {
        if (!isActive()) return;
        // Custom rendering
        UIElement::render(renderer); // Render children
    }
};
```

## Components

### Core Classes

- **Logger**: Unified logging system with multiple log levels
- **InputState**: Abstraction for mouse and keyboard input
- **UIElement**: Base class for all UI elements
- **MainWindow**: Root window element
- **Renderer**: Abstract rendering interface
- **SDLRenderer**: SDL2 implementation of Renderer
- **Theme**: Manages colors, fonts, and visual configuration

### UI Components

- **Button**: Clickable button with hover states
- **Panel**: Container for grouping elements

## Theme Configuration

Create a `theme.json` file:

```json
{
  "colors": {
    "background": [30, 30, 30, 255],
    "button": [70, 130, 180, 255],
    "text": [255, 255, 255, 255]
  },
  "fonts": {
    "default": {
      "path": "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
      "size": 16
    }
  }
}
```

Load in code:
```cpp
Theme::getInstance().loadFromFile("theme.json");
```

## Design Principles

This library strictly follows:

- **RAII**: All resources managed through object lifetimes
- **Smart Pointers**: `std::unique_ptr` and `std::shared_ptr` for memory safety
- **SOLID**:
  - Single Responsibility: Each class has one purpose
  - Open/Closed: Extend via inheritance, not modification
  - Liskov Substitution: Derived classes are substitutable
  - Interface Segregation: Focused interfaces
  - Dependency Inversion: Depend on abstractions

## Documentation

- [DEPENDENCIES.md](DEPENDENCIES.md) - Build and runtime dependencies
- [.github/copilot-instructions.md](.github/copilot-instructions.md) - Coding guidelines and patterns

## License

See [LICENSE](LICENSE) file for details.
