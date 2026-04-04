#include "Renderer.hpp"

#include "../logging/Logger.hpp"

#include <GLES3/gl3.h>

namespace DefenderOS {

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize(int width, int height) {
    if (m_initialized) {
        LOG_WARNING("Renderer::initialize() called more than once");
        return true;
    }

    m_width = width;
    m_height = height;

    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_initialized = true;
    LOG_INFO("Renderer initialised [" +
             std::to_string(width) + "x" + std::to_string(height) + "]");
    return true;
}

void Renderer::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
    // Frame finalisation — buffer swap is the window's responsibility.
}

void Renderer::shutdown() {
    if (!m_initialized) {
        return;
    }
    m_initialized = false;
    LOG_INFO("Renderer shut down");
}

void Renderer::onResize(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
    LOG_DEBUG("Renderer viewport updated to " +
              std::to_string(width) + "x" + std::to_string(height));
}

} // namespace DefenderOS
