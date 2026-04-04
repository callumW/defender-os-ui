#include "EventLoop.hpp"

#include "../logging/Logger.hpp"
#include "../renderer/IRenderer.hpp"
#include "../window/IWindow.hpp"

#include <stdexcept>

namespace DefenderOS {

EventLoop::EventLoop(std::shared_ptr<IWindow> window,
                     std::shared_ptr<IRenderer> renderer)
    : m_window(std::move(window))
    , m_renderer(std::move(renderer)) {
    if (!m_window) {
        LOG_ERROR("EventLoop: null window provided");
        throw std::invalid_argument("window cannot be null");
    }
    if (!m_renderer) {
        LOG_ERROR("EventLoop: null renderer provided");
        throw std::invalid_argument("renderer cannot be null");
    }
}

void EventLoop::setUpdateCallback(std::function<void()> callback) {
    m_updateCallback = std::move(callback);
}

void EventLoop::setRenderCallback(std::function<void()> callback) {
    m_renderCallback = std::move(callback);
}

void EventLoop::run() {
    if (m_running) {
        LOG_WARNING("EventLoop::run() called while already running");
        return;
    }

    m_running = true;
    LOG_INFO("EventLoop started");

    while (m_running && !m_window->shouldClose()) {
        m_window->pollEvents();

        if (m_updateCallback) {
            m_updateCallback();
        }

        m_renderer->beginFrame();

        if (m_renderCallback) {
            m_renderCallback();
        }

        m_renderer->endFrame();
        m_window->swapBuffers();
    }

    m_running = false;
    LOG_INFO("EventLoop stopped");
}

void EventLoop::stop() {
    m_running = false;
    LOG_INFO("EventLoop stop requested");
}

bool EventLoop::isRunning() const {
    return m_running;
}

} // namespace DefenderOS
