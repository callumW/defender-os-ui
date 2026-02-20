# Architecture Overview

## Design Philosophy

The DefenderOS UI Library follows modern C++ best practices with a focus on:

1. **Safety**: RAII and smart pointers prevent memory leaks
2. **Simplicity**: Each function has a single, clear purpose
3. **Extensibility**: Easy to add new UI components via inheritance
4. **Performance**: Only active elements are updated/rendered
5. **Maintainability**: SOLID principles throughout

## Core Concepts

### Tree Structure

The UI system is organized as a tree:

```
MainWindow (root)
├── Panel
│   ├── Button 1
│   ├── Button 2
│   └── Button 3
└── Panel
    └── Button 4
```

**Key Properties:**
- Each element has one parent (except root)
- Elements can have multiple children
- Coordinates are relative to parent
- States propagate down the tree

### Active/Inactive States

Elements can be active or inactive:

- **Active**: Updated and rendered each frame
- **Inactive**: Skipped entirely (performance optimization)

Setting a parent inactive automatically deactivates all children.

**Use Case**: In a menu system, only show the current menu panel:

```cpp
mainMenuPanel->setActive(false);
settingsPanel->setActive(true); // Show settings instead
```

### Frame Loop

The application runs in a continuous loop:

```
┌─────────────────────────────────┐
│ 1. Process Input Events         │
│    - Mouse, keyboard, etc.      │
│    - Update InputState          │
└────────────┬────────────────────┘
             │
┌────────────▼────────────────────┐
│ 2. Update Active Elements       │
│    - Check input state          │
│    - Update logic               │
│    - Recursively update children│
└────────────┬────────────────────┘
             │
┌────────────▼────────────────────┐
│ 3. Render Active Elements       │
│    - Clear screen               │
│    - Render UI tree             │
│    - Present frame              │
└─────────────────────────────────┘
```

## Class Hierarchy

### UIElement (Base Class)

All UI components inherit from `UIElement`:

```cpp
class UIElement {
    // Position and size (relative to parent)
    int x_, y_, width_, height_;
    
    // Tree structure
    UIElement* parent_;
    vector<unique_ptr<UIElement>> children_;
    
    // State
    bool isActive_;
    
    // Virtual methods for customization
    virtual void update(const InputState& input, float deltaTime);
    virtual void render(Renderer& renderer);
};
```

**Derived Classes:**
- `MainWindow`: Root element with SDL window
- `Button`: Clickable button with callbacks
- `Panel`: Container for grouping elements

### Resource Management

All SDL resources use RAII wrappers:

```cpp
// SDL_Window wrapped in unique_ptr with custom deleter
unique_ptr<SDL_Window, SDLWindowDeleter> window_;

// Deleter automatically calls SDL_DestroyWindow
struct SDLWindowDeleter {
    void operator()(SDL_Window* window) const {
        if (window) SDL_DestroyWindow(window);
    }
};
```

**Benefits:**
- No manual cleanup needed
- Exception-safe
- No memory leaks

### Renderer Abstraction

The `Renderer` is an abstract interface:

```cpp
class Renderer {
public:
    virtual void clear() = 0;
    virtual void fillRect(int x, int y, int w, int h) = 0;
    virtual void drawText(...) = 0;
    // ...
};
```

**Implementation:**
- `SDLRenderer`: SDL2 implementation

**Benefits:**
- Easy to add new rendering backends
- Testable with mock renderers
- Dependency inversion principle

## SOLID Principles Applied

### Single Responsibility Principle

Each class has one reason to change:

- `Logger`: Only handles logging
- `Theme`: Only manages theme data
- `Button`: Only represents a button
- `UIElement`: Only manages element tree structure

### Open/Closed Principle

Classes are open for extension, closed for modification:

```cpp
// Extend UIElement to create new components
class MyCustomElement : public UIElement {
    void update(...) override { /* custom behavior */ }
    void render(...) override { /* custom rendering */ }
};
```

No need to modify `UIElement` base class.

### Liskov Substitution Principle

Derived classes are substitutable:

```cpp
void processElement(UIElement* element) {
    element->update(input, dt);  // Works with any UIElement
    element->render(renderer);
}

// Works with Button, Panel, or any UIElement derivative
processElement(myButton.get());
processElement(myPanel.get());
```

### Interface Segregation Principle

Interfaces are focused and minimal:

- `Renderer`: Only rendering methods
- `InputState`: Only input data (no rendering logic)
- `Logger`: Only logging methods

### Dependency Inversion Principle

Depend on abstractions, not concretions:

```cpp
// UIElement depends on abstract Renderer
void UIElement::render(Renderer& renderer);  // Not SDLRenderer

// Can use any Renderer implementation
SDLRenderer sdlRenderer;
MockRenderer mockRenderer;  // For testing
```

## Input Handling

### InputState Abstraction

Input events are abstracted into a queryable state:

```cpp
InputState input;

// In event loop
mainWindow.processEvents(input);  // Updates input state

// In update methods
if (input.getMouse().leftClicked) {
    // Handle click
}
```

**Benefits:**
- Decouples input from SDL events
- Easy to test with mock input
- Consistent interface for all elements

### Per-Frame vs Persistent State

```cpp
// Persistent (stays true while held)
input.getMouse().leftButton

// Per-frame (true only on frame of click)
input.getMouse().leftClicked

// Reset per-frame states each frame
input.reset();
```

## Theme System

Themes provide centralized style management:

```cpp
// Get colors from theme
Color buttonColor = Theme::getInstance().getColor("button");

// Fonts
string fontPath = Theme::getInstance().getFontPath("default");
int fontSize = Theme::getInstance().getFontSize("default");
```

**Configurable via JSON:**
```json
{
  "colors": {
    "button": [70, 130, 180, 255]
  },
  "fonts": {
    "default": {
      "path": "/path/to/font.ttf",
      "size": 16
    }
  }
}
```

## Memory Management

### Smart Pointers

```cpp
// Use unique_ptr for exclusive ownership
unique_ptr<Button> button = make_unique<Button>();

// Move ownership when adding to parent
parent->addChild(move(button));

// Parent owns button, will clean up automatically
```

### No Raw Pointers for Ownership

Raw pointers are only used for non-owning references:

```cpp
// Raw pointer to parent (parent owns itself elsewhere)
UIElement* parent_;

// But children are owned via unique_ptr
vector<unique_ptr<UIElement>> children_;
```

## Extension Guide

### Creating a Custom UI Element

```cpp
class TextField : public UIElement {
public:
    TextField() : UIElement() {
        setName("TextField");
        setSize(200, 30);
    }

    void update(const InputState& input, float deltaTime) override {
        if (!isActive()) return;
        
        // Custom input handling
        if (input.getKeyboard().enterPressed) {
            // Process text input
        }
        
        // Don't forget to update children!
        UIElement::update(input, deltaTime);
    }

    void render(Renderer& renderer) override {
        if (!isActive()) return;
        
        int absX, absY;
        getAbsolutePosition(absX, absY);
        
        // Draw background
        renderer.setDrawColor(Theme::getInstance().getColor("panel"));
        renderer.fillRect(absX, absY, width_, height_);
        
        // Draw text
        renderer.drawText(text_, absX + 5, absY + 5, 
                         "default", Theme::getInstance().getColor("text"));
        
        // Don't forget to render children!
        UIElement::render(renderer);
    }

private:
    string text_;
};
```

### Adding New Renderer Backend

```cpp
class VulkanRenderer : public Renderer {
public:
    void clear() override {
        // Vulkan-specific clear
    }
    
    void fillRect(int x, int y, int w, int h) override {
        // Vulkan-specific rectangle drawing
    }
    
    // Implement other virtual methods...
};
```

## Performance Considerations

1. **Active/Inactive States**: Avoid updating/rendering unused elements
2. **Relative Positioning**: Efficient coordinate calculations
3. **Smart Pointer Overhead**: Minimal (one pointer per unique_ptr)
4. **Virtual Function Calls**: Negligible for UI (not in tight loops)
5. **Font Caching**: Fonts loaded once and cached

## Testing Strategy

Each component has focused unit tests:

- **Logger**: Singleton, log levels, output
- **UIElement**: Tree operations, state propagation, positioning
- **Theme**: Color/font management
- **InputState**: State tracking, reset behavior

Mock objects used for testing without SDL:

```cpp
class MockRenderer : public Renderer {
    // Minimal implementation for testing
};
```

## Future Enhancements

Potential additions while maintaining current architecture:

1. **More UI Components**: TextBox, Checkbox, Slider, etc.
2. **Layout Managers**: Automatic positioning/sizing
3. **Animations**: Smooth transitions between states
4. **Touch Input**: Extend InputState for touch events
5. **Styling**: CSS-like styling system
6. **JSON Loading**: Full theme loading from JSON
7. **Resource Manager**: Centralized texture/font management
