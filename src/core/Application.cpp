#include "Application.hpp"

#include "../logging/Logger.hpp"
#include "../renderer/IRenderer.hpp"
#include "../window/IWindow.hpp"
#include "EventLoop.hpp"

#include <stdexcept>

namespace DefenderOS {

Application::Application(std::shared_ptr<IWindow> window,
                         std::shared_ptr<IRenderer> renderer)
    : m_window(std::move(window))
    , m_renderer(std::move(renderer)) {
    if (!m_window) {
        throw std::invalid_argument("window cannot be null");
    }
    if (!m_renderer) {
        throw std::invalid_argument("renderer cannot be null");
    }
}

Application::~Application() {
    shutdown();
}

bool Application::initialize() {
    if (m_initialized) {
        LOG_WARNING("Application::initialize() called more than once");
        return true;
    }

    LOG_INFO("Initialising DefenderOS UI");

    if (!m_window->initialize()) {
        LOG_ERROR("Window initialisation failed");
        return false;
    }

    if (!m_renderer->initialize(m_window->getWidth(), m_window->getHeight())) {
        LOG_ERROR("Renderer initialisation failed");
        m_window->shutdown();
        return false;
    }

    m_eventLoop = std::make_unique<EventLoop>(m_window, m_renderer);

    m_initialized = true;
    LOG_INFO("Application initialised successfully");
    return true;
}

void Application::run() {
    if (!m_initialized) {
        LOG_ERROR("Application::run() called before initialize()");
        return;
    }
    LOG_INFO("Entering event loop");
    m_eventLoop->run();
}

void Application::shutdown() {
    if (!m_initialized) {
        return;
    }

    LOG_INFO("Shutting down application");

    m_eventLoop.reset();

    if (m_renderer) {
        m_renderer->shutdown();
    }
    if (m_window) {
        m_window->shutdown();
    }

    m_initialized = false;
    LOG_INFO("Application shut down");
}

} // namespace DefenderOS
