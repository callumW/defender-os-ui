# GitHub Copilot Instructions — defender-os-ui

## Project Overview

`defender-os-ui` is a C++17 UI library for embedded Linux systems using **GLFW** for windowing and **OpenGL ES 3.0** for rendering. It targets full-screen kiosk-style applications on devices such as the Raspberry Pi and similar SBCs.

---

## Language & Standard

- **C++17** — use all C++17 features freely (structured bindings, `if constexpr`, `std::filesystem`, etc.)
- All new source files must begin with the project namespace: `namespace DefenderOS { ... }`
- Do **not** use `using namespace std;` in header files

---

## Code Principles

### SOLID
- **Single Responsibility**: every class has exactly one reason to change
- **Open/Closed**: extend behaviour through interfaces and composition, not by modifying existing classes
- **Liskov Substitution**: derived classes must be fully substitutable for their base
- **Interface Segregation**: prefer narrow, focused interfaces over wide monolithic ones
- **Dependency Inversion**: high-level modules depend on abstractions (`IWindow`, `IRenderer`), not concrete implementations

### RAII
- Acquire resources in constructors; release them in destructors
- Never use raw `new` / `delete`; prefer `std::make_unique` / `std::make_shared`
- Mark classes non-copyable when they own exclusive resources

### Smart Pointers
- `std::unique_ptr` for exclusive ownership
- `std::shared_ptr` for shared ownership (e.g., Window and Renderer shared between Application and EventLoop)
- `std::weak_ptr` to break ownership cycles
- Never store raw owning pointers

---

## Naming Conventions

| Item | Convention | Example |
|------|-----------|---------|
| Class | `PascalCase` | `EventLoop` |
| Interface | `IPascalCase` | `IWindow` |
| Member variable | `m_camelCase` | `m_window` |
| Free function / method | `camelCase` | `initialize()` |
| Constant | `SCREAMING_SNAKE` | `MAX_FRAMES` |
| Enum value | `SCREAMING_SNAKE` in `enum class` | `LogLevel::DEBUG` |
| File | `PascalCase.hpp/.cpp` | `Window.hpp` |

---

## Function Design

- Keep functions **short** (aim for < 20 lines of logic)
- Each function does **one thing**
- Prefer `[[nodiscard]]` on factory and status-returning functions
- Use `const` everywhere it is correct

---

## Logging

Use the global `DefenderOS::Logger` singleton for all log output:

```cpp
LOG_DEBUG("msg");      // filtered out unless level >= DEBUG
LOG_INFO("msg");
LOG_WARNING("msg");
LOG_ERROR("msg");
LOG_CRITICAL("msg");
```

Provide a module name where helpful:
```cpp
Logger::instance().info("Window created", "Window");
```

Do **not** use `std::cout` or `fprintf` directly in library code.

---

## Error Handling

- Return `bool` or `std::optional` for recoverable failures in `initialize()` methods
- Throw `std::invalid_argument` for programming errors caught at construction time (e.g., null dependencies)
- Log all errors before returning `false` or throwing
- Do **not** use exception specifications (`throw(...)`)

---

## Interfaces

All platform-specific subsystems must be hidden behind pure virtual interfaces:

- `IWindow` — windowing & input
- `IRenderer` — rendering context

This enables unit testing via mock implementations without requiring a GPU or display.

---

## Testing

- **Framework**: GoogleTest (via CMake `FetchContent`)
- **Runner**: CTest (`cmake --build build --target test` or `ctest --test-dir build`)
- Every public, non-GPU function **must** have at least one unit test
- GPU-dependent code (`Window`, `Renderer`) is tested through mock interfaces
- Test files live in `tests/`, named `test_<module>.cpp`
- Mocks live in `tests/mocks/`, named `Mock<Interface>.hpp`

---

## Build System

- **CMake 3.20+** required
- Two library targets:
  - `DefenderOsUICore` — Logger, EventLoop, Application (no GPU deps)
  - `DefenderOsUIPlatform` — Window (GLFW), Renderer (GLES), ApplicationBuilder
- External dependencies fetched via `FetchContent` (GLFW, GoogleTest)
- GLES/EGL via system `pkg-config`

---

## Dependencies

See [`dependencies.md`](../dependencies.md) for the full dependency list with versions and justifications.

---

## File Layout

```
src/
  logging/    — Logger (singleton, thread-safe)
  core/       — Application, EventLoop, interfaces
  window/     — IWindow, Window (GLFW)
  renderer/   — IRenderer, Renderer (OpenGL ES 3)
  platform/   — ApplicationBuilder (production factory)
tests/
  mocks/      — MockWindow, MockRenderer
  test_*.cpp  — unit tests
```
