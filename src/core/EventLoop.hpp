#pragma once

#include <functional>
#include <memory>

namespace DefenderOS {

class IWindow;
class IRenderer;

/// Drives the main event-processing and rendering loop.
///
/// The loop runs until the window signals it should close or stop() is called.
/// Optional callbacks let callers inject per-frame update and render logic
/// without subclassing.
class EventLoop {
public:
    /// @throws std::invalid_argument if @p window or @p renderer is null.
    EventLoop(std::shared_ptr<IWindow> window,
              std::shared_ptr<IRenderer> renderer);

    ~EventLoop() = default;

    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;

    /// Registers a callback invoked once per frame before rendering begins.
    void setUpdateCallback(std::function<void()> callback);

    /// Registers a callback invoked once per frame between beginFrame/endFrame.
    void setRenderCallback(std::function<void()> callback);

    /// Blocks until the loop exits.
    void run();

    /// Signals the loop to exit after the current frame completes.
    void stop();

    [[nodiscard]] bool isRunning() const;

private:
    std::shared_ptr<IWindow> m_window;
    std::shared_ptr<IRenderer> m_renderer;
    bool m_running{false};
    std::function<void()> m_updateCallback;
    std::function<void()> m_renderCallback;
};

} // namespace DefenderOS
