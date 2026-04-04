#pragma once

#include "../../src/text/ITextRenderer.hpp"

#include <atomic>
#include <string>

namespace DefenderOS::Test {

/// Test double for ITextRenderer. All state is public for easy inspection.
class MockTextRenderer : public ITextRenderer {
public:
    bool initialize(int width, int height) override {
        m_initializeCalled = true;
        m_width = width;
        m_height = height;
        if (m_initializeResult) {
            m_isReady = true;
        }
        return m_initializeResult;
    }

    bool loadFont(const std::string& fontPath, unsigned int pixelSize) override {
        m_loadFontCalled = true;
        m_lastFontPath = fontPath;
        m_lastPixelSize = pixelSize;
        return m_loadFontResult;
    }

    void renderText(const std::string& text,
                    float x, float y, float scale,
                    float r, float g, float b, float a = 1.0f) override {
        ++m_renderTextCount;
        m_lastText = text;
        m_lastX = x;
        m_lastY = y;
        m_lastScale = scale;
        m_lastR = r;
        m_lastG = g;
        m_lastB = b;
        m_lastA = a;
    }

    void onResize(int width, int height) override {
        m_width = width;
        m_height = height;
        ++m_onResizeCount;
    }

    void shutdown() override { m_shutdownCalled = true; }

    bool isReady() const override { return m_isReady; }

    // --- controllable state -------------------------------------------------
    bool m_initializeResult{true};
    bool m_loadFontResult{true};
    bool m_initializeCalled{false};
    bool m_loadFontCalled{false};
    bool m_shutdownCalled{false};
    bool m_isReady{false};
    std::atomic<int> m_renderTextCount{0};
    std::atomic<int> m_onResizeCount{0};
    std::string m_lastFontPath;
    std::string m_lastText;
    unsigned int m_lastPixelSize{0};
    float m_lastX{0.0f};
    float m_lastY{0.0f};
    float m_lastScale{0.0f};
    float m_lastR{0.0f};
    float m_lastG{0.0f};
    float m_lastB{0.0f};
    float m_lastA{0.0f};
    int m_width{0};
    int m_height{0};
};

} // namespace DefenderOS::Test
