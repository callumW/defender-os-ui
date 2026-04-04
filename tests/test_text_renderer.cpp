#include <gtest/gtest.h>

#include "../src/text/ITextRenderer.hpp"
#include "mocks/MockTextRenderer.hpp"

using namespace DefenderOS;
using namespace DefenderOS::Test;

class TextRendererTest : public ::testing::Test {
protected:
    MockTextRenderer renderer;
};

// --- isReady ----------------------------------------------------------------

TEST_F(TextRendererTest, NotReadyBeforeInitialize) {
    EXPECT_FALSE(renderer.isReady());
}

TEST_F(TextRendererTest, ReadyAfterSuccessfulInitialize) {
    EXPECT_TRUE(renderer.initialize(1920, 1080));
    EXPECT_TRUE(renderer.isReady());
}

TEST_F(TextRendererTest, NotReadyWhenInitializeFails) {
    renderer.m_initializeResult = false;
    EXPECT_FALSE(renderer.initialize(1920, 1080));
    EXPECT_FALSE(renderer.isReady());
}

// --- initialize -------------------------------------------------------------

TEST_F(TextRendererTest, InitializeStoresViewportDimensions) {
    renderer.initialize(1280, 720);
    EXPECT_EQ(renderer.m_width,  1280);
    EXPECT_EQ(renderer.m_height, 720);
}

TEST_F(TextRendererTest, InitializeSetsCalled) {
    renderer.initialize(800, 600);
    EXPECT_TRUE(renderer.m_initializeCalled);
}

// --- loadFont ---------------------------------------------------------------

TEST_F(TextRendererTest, LoadFontRecordsPathAndSize) {
    renderer.initialize(1920, 1080);
    renderer.loadFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    EXPECT_EQ(renderer.m_lastFontPath,
              "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    EXPECT_EQ(renderer.m_lastPixelSize, 24u);
}

TEST_F(TextRendererTest, LoadFontReturnsTrueByDefault) {
    renderer.initialize(1920, 1080);
    EXPECT_TRUE(renderer.loadFont("any/path.ttf", 32));
}

TEST_F(TextRendererTest, LoadFontCanBeConfiguredToFail) {
    renderer.m_loadFontResult = false;
    renderer.initialize(1920, 1080);
    EXPECT_FALSE(renderer.loadFont("missing.ttf", 16));
}

TEST_F(TextRendererTest, LoadFontSetsCalled) {
    renderer.initialize(1920, 1080);
    renderer.loadFont("font.ttf", 12);
    EXPECT_TRUE(renderer.m_loadFontCalled);
}

// --- renderText -------------------------------------------------------------

TEST_F(TextRendererTest, RenderTextTracksCallCount) {
    renderer.initialize(1920, 1080);
    EXPECT_EQ(renderer.m_renderTextCount.load(), 0);
    renderer.renderText("Hello", 10.0f, 20.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    EXPECT_EQ(renderer.m_renderTextCount.load(), 1);
    renderer.renderText("World", 10.0f, 50.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    EXPECT_EQ(renderer.m_renderTextCount.load(), 2);
}

TEST_F(TextRendererTest, RenderTextStoresLastArguments) {
    renderer.initialize(1920, 1080);
    renderer.renderText("Test", 100.0f, 200.0f, 2.0f, 0.5f, 0.25f, 0.75f, 0.9f);
    EXPECT_EQ(renderer.m_lastText,  "Test");
    EXPECT_FLOAT_EQ(renderer.m_lastX,     100.0f);
    EXPECT_FLOAT_EQ(renderer.m_lastY,     200.0f);
    EXPECT_FLOAT_EQ(renderer.m_lastScale,   2.0f);
    EXPECT_FLOAT_EQ(renderer.m_lastR,       0.5f);
    EXPECT_FLOAT_EQ(renderer.m_lastG,      0.25f);
    EXPECT_FLOAT_EQ(renderer.m_lastB,      0.75f);
    EXPECT_FLOAT_EQ(renderer.m_lastA,       0.9f);
}

TEST_F(TextRendererTest, RenderTextEmptyStringDoesNotCrash) {
    renderer.initialize(1920, 1080);
    EXPECT_NO_THROW(renderer.renderText("", 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f));
}

TEST_F(TextRendererTest, RenderTextDefaultAlphaIsOne) {
    renderer.initialize(1920, 1080);
    renderer.renderText("A", 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(renderer.m_lastA, 1.0f);
}

// --- onResize ---------------------------------------------------------------

TEST_F(TextRendererTest, OnResizeUpdatesViewportDimensions) {
    renderer.initialize(1920, 1080);
    renderer.onResize(2560, 1440);
    EXPECT_EQ(renderer.m_width,  2560);
    EXPECT_EQ(renderer.m_height, 1440);
}

TEST_F(TextRendererTest, OnResizeTracksCallCount) {
    renderer.initialize(1920, 1080);
    renderer.onResize(800, 600);
    renderer.onResize(1024, 768);
    EXPECT_EQ(renderer.m_onResizeCount.load(), 2);
}

// --- shutdown ---------------------------------------------------------------

TEST_F(TextRendererTest, ShutdownSetsCalled) {
    renderer.initialize(1920, 1080);
    renderer.shutdown();
    EXPECT_TRUE(renderer.m_shutdownCalled);
}
