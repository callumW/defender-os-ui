#pragma once

#include "../../src/renderer/IRenderer.hpp"

#include <atomic>

namespace DefenderOS::Test {

/// Test double for IRenderer.  All state is public for easy inspection.
class MockRenderer : public IRenderer {
public:
    bool initialize(int width, int height) override {
        m_initializeCalled = true;
        m_width = width;
        m_height = height;
        return m_initializeResult;
    }

    void beginFrame() override { ++m_beginFrameCount; }
    void endFrame() override   { ++m_endFrameCount; }

    void shutdown() override { m_shutdownCalled = true; }

    void onResize(int width, int height) override {
        m_width = width;
        m_height = height;
    }

    // --- controllable state -------------------------------------------------
    bool m_initializeResult{true};
    bool m_initializeCalled{false};
    bool m_shutdownCalled{false};
    std::atomic<int> m_beginFrameCount{0};
    std::atomic<int> m_endFrameCount{0};
    int m_width{0};
    int m_height{0};
};

} // namespace DefenderOS::Test
