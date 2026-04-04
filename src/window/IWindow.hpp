#pragma once

namespace DefenderOS {

/// Abstract interface for windowing and input event polling.
///
/// Concrete implementations (e.g. Window using GLFW) must satisfy all methods.
/// The interface is kept narrow so mock implementations are trivial to write.
class IWindow {
public:
    virtual ~IWindow() = default;

    /// Initialise the underlying platform window. Returns false on failure.
    [[nodiscard]] virtual bool initialize() = 0;

    /// Release all windowing resources.
    virtual void shutdown() = 0;

    /// Returns true when the user has requested the window to close.
    [[nodiscard]] virtual bool shouldClose() const = 0;

    /// Process all pending input and window events.
    virtual void pollEvents() = 0;

    /// Swap the front and back buffers (presents the rendered frame).
    virtual void swapBuffers() = 0;

    [[nodiscard]] virtual int getWidth() const = 0;
    [[nodiscard]] virtual int getHeight() const = 0;

    /// Returns the underlying native window handle (e.g. GLFWwindow*).
    [[nodiscard]] virtual void* getNativeHandle() const = 0;
};

} // namespace DefenderOS
