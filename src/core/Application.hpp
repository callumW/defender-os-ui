#pragma once

#include <memory>
#include <string>

namespace DefenderOS {

class IWindow;
class IRenderer;
class EventLoop;

/// Top-level application orchestrator.
///
/// The Application owns the EventLoop and coordinates the lifecycle of the
/// window and renderer.  Dependencies are injected via the constructor so that
/// unit tests can supply mocks without requiring a GPU or display.
///
/// Production code should use ApplicationBuilder (src/platform/) to construct
/// a fully configured Application with real GLFW/GLES resources.
class Application {
public:
    /// Inject pre-constructed window and renderer dependencies.
    Application(std::shared_ptr<IWindow> window,
                std::shared_ptr<IRenderer> renderer);

    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /// Initialise all subsystems. Must be called before run().
    /// Returns false and logs the reason on any failure.
    [[nodiscard]] bool initialize();

    /// Enter the event/render loop. Blocks until the window closes or
    /// stop() is called from a callback.
    void run();

    /// Tear down all subsystems. Safe to call multiple times.
    void shutdown();

private:
    std::shared_ptr<IWindow> m_window;
    std::shared_ptr<IRenderer> m_renderer;
    std::unique_ptr<EventLoop> m_eventLoop;
    bool m_initialized{false};
};

} // namespace DefenderOS
