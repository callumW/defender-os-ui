#pragma once

#include "../window/IWindow.hpp"

#include <string>

struct GLFWwindow;

namespace DefenderOS {

struct WindowConfig {
    std::string title{"DefenderOS UI"};
    bool fullscreen{true};
    int width{1920};
    int height{1080};
};

/// RAII wrapper around a GLFWwindow configured for an OpenGL ES 3.0 context.
///
/// Fullscreen mode queries the primary monitor's native resolution at
/// initialisation time, so width/height in the config are only used when
/// fullscreen is false.
class Window : public IWindow {
public:
    explicit Window(WindowConfig config = {});
    ~Window() override;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] bool initialize() override;
    void shutdown() override;
    [[nodiscard]] bool shouldClose() const override;
    void pollEvents() override;
    void swapBuffers() override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
    [[nodiscard]] void* getNativeHandle() const override;

private:
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    WindowConfig m_config;
    GLFWwindow* m_window{nullptr};
    bool m_initialized{false};
};

} // namespace DefenderOS
