#include "Window.hpp"

#include "../logging/Logger.hpp"

#include <GLFW/glfw3.h>

namespace DefenderOS {

Window::Window(WindowConfig config)
    : m_config(std::move(config)) {}

Window::~Window() {
    shutdown();
}

bool Window::initialize() {
    if (m_initialized) {
        LOG_WARNING("Window::initialize() called more than once");
        return true;
    }

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialise GLFW");
        return false;
    }

    // Request an OpenGL ES 3.0 context via the EGL context creation API.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWmonitor* monitor = nullptr;
    int width = m_config.width;
    int height = m_config.height;

    if (m_config.fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        if (monitor) {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            if (mode) {
                width = mode->width;
                height = mode->height;
                m_config.width = width;
                m_config.height = height;
            }
        }
    }

    m_window = glfwCreateWindow(width, height, m_config.title.c_str(), monitor, nullptr);
    if (!m_window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // enable VSync
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    m_initialized = true;
    LOG_INFO("Window created: " + m_config.title +
             " [" + std::to_string(width) + "x" + std::to_string(height) + "]"
             + (m_config.fullscreen ? " (fullscreen)" : ""));
    return true;
}

void Window::shutdown() {
    if (!m_initialized) {
        return;
    }
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
    m_initialized = false;
    LOG_INFO("Window shut down");
}

bool Window::shouldClose() const {
    return m_window ? glfwWindowShouldClose(m_window) != 0 : true;
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    if (m_window) {
        glfwSwapBuffers(m_window);
    }
}

int Window::getWidth() const {
    return m_config.width;
}

int Window::getHeight() const {
    return m_config.height;
}

void* Window::getNativeHandle() const {
    return m_window;
}

// --- static callbacks -------------------------------------------------------

void Window::errorCallback(int error, const char* description) {
    Logger::instance().error(
        "GLFW error " + std::to_string(error) + ": " + description, "GLFW");
}

void Window::framebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
    Logger::instance().debug(
        "Framebuffer resized to " + std::to_string(width) + "x" + std::to_string(height),
        "Window");

    // Notify the renderer via the user pointer set during initialisation.
    // The renderer is not directly accessible here; resizing is handled by the
    // application layer which observes the window dimensions each frame.
    (void)glfwWindow;
}

} // namespace DefenderOS
