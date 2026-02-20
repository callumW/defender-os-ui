# Copilot Instructions for DefenderOS UI Library

## Code Style and Standards

### General Guidelines
- Use C++17 standard features
- Follow RAII principles - all resources must be managed through object lifetimes
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) instead of raw pointers
- Avoid manual `new`/`delete` - use `std::make_unique` and `std::make_shared`
- Keep functions short and focused (single responsibility)
- Use const-correctness throughout
- Prefer composition over inheritance

### Naming Conventions
- Classes: PascalCase (e.g., `UIElement`, `MainWindow`)
- Functions/Methods: camelCase (e.g., `update()`, `render()`, `setActive()`)
- Member variables: camelCase with trailing underscore (e.g., `parent_`, `children_`)
- Constants: UPPER_SNAKE_CASE (e.g., `MAX_CHILDREN`, `DEFAULT_FONT_SIZE`)
- Namespaces: lowercase (e.g., `defender`, `ui`)

### File Organization
- Header files: `.h` extension in `include/defender/` directory
- Implementation files: `.cpp` extension in `src/` directory
- One class per file (with rare exceptions for tightly coupled classes)
- Header guards: Use `#pragma once`

### SOLID Principles Application

#### Single Responsibility Principle
Each class should have one reason to change:
- `Logger`: Only responsible for logging
- `UIElement`: Only responsible for being a UI element
- `Theme`: Only responsible for theme configuration

#### Open/Closed Principle
Classes should be open for extension but closed for modification:
- Use virtual functions for extensibility
- Derive from `UIElement` to create new UI components
- Don't modify base classes to add new features

#### Liskov Substitution Principle
Derived classes must be substitutable for base classes:
- All derived UI elements must work wherever `UIElement` is expected
- Don't violate base class contracts in derived classes

#### Interface Segregation Principle
Don't force clients to depend on unused interfaces:
- Keep interfaces focused and minimal
- Split large interfaces into smaller, specific ones

#### Dependency Inversion Principle
Depend on abstractions, not concretions:
- Use abstract base classes or interfaces
- Inject dependencies rather than creating them internally

## Architecture Patterns

### Tree Structure
- All UI elements form a tree with `MainWindow` as root
- Each element has a parent (except root) and can have children
- Coordinates and scaling are relative to parent
- Active/inactive state propagates down the tree

### Frame Loop Pattern
```cpp
while (running) {
    // 1. Process input events
    inputState.update(events);
    
    // 2. Update all active elements
    mainWindow->update(inputState, deltaTime);
    
    // 3. Render all active elements
    renderer->clear();
    mainWindow->render(renderer);
    renderer->present();
}
```

### Resource Management
- Use RAII wrappers for SDL resources (SDL_Window, SDL_Renderer, etc.)
- Resources are automatically cleaned up when objects go out of scope
- Example:
```cpp
class SDLWindow {
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
public:
    SDLWindow() : window_(SDL_CreateWindow(...), SDL_DestroyWindow) {}
    // No manual cleanup needed - handled by unique_ptr
};
```

## Testing Guidelines

### Unit Testing with CTest
- Every public function should have unit tests
- Test files go in `tests/` directory
- Test file naming: `test_<component>.cpp`
- Use assertion macros for validation
- Test edge cases and error conditions

### Test Structure
```cpp
// tests/test_ui_element.cpp
#include "ui_element.h"
#include <cassert>

void testUIElementCreation() {
    auto element = std::make_unique<UIElement>();
    assert(element != nullptr);
}

int main() {
    testUIElementCreation();
    return 0;
}
```

## Logging System

### Usage
```cpp
#include "logger.h"

// Log at different levels
Logger::debug("Debug message: value = {}", value);
Logger::info("Operation completed successfully");
Logger::warning("Resource usage is high");
Logger::error("Failed to load file: {}", filename);
```

### Log Levels
- DEBUG: Detailed diagnostic information
- INFO: General informational messages
- WARNING: Warning messages for potentially harmful situations
- ERROR: Error messages for serious problems

## Common Patterns

### Creating a New UI Element
```cpp
class Button : public UIElement {
public:
    Button() : UIElement() {}
    
    void update(const InputState& input, float deltaTime) override {
        if (!isActive()) return;
        // Update logic
        UIElement::update(input, deltaTime); // Update children
    }
    
    void render(Renderer& renderer) override {
        if (!isActive()) return;
        // Render logic
        UIElement::render(renderer); // Render children
    }
};
```

### Adding Children
```cpp
auto parent = std::make_unique<Panel>();
auto button = std::make_unique<Button>();
parent->addChild(std::move(button));
```

## Performance Considerations

- Minimize allocations in the frame loop
- Use move semantics for transferring ownership
- Avoid unnecessary copying (use references)
- Cache frequently accessed values
- Only update/render active elements

## Error Handling

- Use exceptions for initialization/configuration errors
- Return error codes or std::optional for runtime errors
- Log errors before throwing exceptions
- Never throw exceptions in destructors

## Documentation

- Use doxygen-style comments for public APIs
- Document preconditions and postconditions
- Explain non-obvious design decisions
- Keep comments up-to-date with code changes
