#pragma once

#include "../../src/window/IWindow.hpp"

#include <atomic>

namespace DefenderOS::Test {

/// Test double for IWindow.  All state is public for easy inspection.
class MockWindow : public IWindow {
public:
    bool initialize() override {
        m_initializeCalled = true;
        return m_initializeResult;
    }

    void shutdown() override {
        m_shutdownCalled = true;
    }

    bool shouldClose() const override {
        return m_shouldClose.load();
    }

    void pollEvents() override {
        ++m_pollCount;
    }

    void swapBuffers() override {
        ++m_swapCount;
    }

    int getWidth() const override { return m_width; }
    int getHeight() const override { return m_height; }
    void* getNativeHandle() const override { return nullptr; }

    // --- controllable state -------------------------------------------------
    bool m_initializeResult{true};
    bool m_initializeCalled{false};
    bool m_shutdownCalled{false};
    std::atomic<bool> m_shouldClose{false};
    std::atomic<int> m_pollCount{0};
    std::atomic<int> m_swapCount{0};
    int m_width{1920};
    int m_height{1080};
};

} // namespace DefenderOS::Test
