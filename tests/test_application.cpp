#include <gtest/gtest.h>

#include "../src/core/Application.hpp"
#include "mocks/MockRenderer.hpp"
#include "mocks/MockWindow.hpp"

using namespace DefenderOS;
using namespace DefenderOS::Test;

class ApplicationTest : public ::testing::Test {
protected:
    std::shared_ptr<MockWindow> window;
    std::shared_ptr<MockRenderer> renderer;

    void SetUp() override {
        window = std::make_shared<MockWindow>();
        renderer = std::make_shared<MockRenderer>();
    }
};

TEST_F(ApplicationTest, ThrowsOnNullWindow) {
    EXPECT_THROW(Application(nullptr, renderer), std::invalid_argument);
}

TEST_F(ApplicationTest, ThrowsOnNullRenderer) {
    EXPECT_THROW(Application(window, nullptr), std::invalid_argument);
}

TEST_F(ApplicationTest, InitializesSuccessfully) {
    Application app(window, renderer);
    EXPECT_TRUE(app.initialize());
    EXPECT_TRUE(window->m_initializeCalled);
    EXPECT_TRUE(renderer->m_initializeCalled);
}

TEST_F(ApplicationTest, InitializeReturnsFalseWhenWindowFails) {
    window->m_initializeResult = false;
    Application app(window, renderer);
    EXPECT_FALSE(app.initialize());
    EXPECT_FALSE(renderer->m_initializeCalled);
}

TEST_F(ApplicationTest, InitializeReturnsFalseWhenRendererFails) {
    renderer->m_initializeResult = false;
    Application app(window, renderer);
    EXPECT_FALSE(app.initialize());
    // Window should be shut down on renderer failure.
    EXPECT_TRUE(window->m_shutdownCalled);
}

TEST_F(ApplicationTest, InitializeIsIdempotent) {
    Application app(window, renderer);
    EXPECT_TRUE(app.initialize());
    // Second call returns true without re-initialising subsystems.
    EXPECT_TRUE(app.initialize());
    EXPECT_TRUE(window->m_initializeCalled);
}

TEST_F(ApplicationTest, RunWithoutInitializeDoesNotCrash) {
    Application app(window, renderer);
    EXPECT_NO_THROW(app.run());
}

TEST_F(ApplicationTest, RunExitsWhenWindowCloses) {
    window->m_shouldClose = true;
    Application app(window, renderer);
    EXPECT_TRUE(app.initialize());
    EXPECT_NO_THROW(app.run());
}

TEST_F(ApplicationTest, ShutdownIsIdempotent) {
    Application app(window, renderer);
    EXPECT_TRUE(app.initialize());
    EXPECT_NO_THROW(app.shutdown());
    EXPECT_NO_THROW(app.shutdown());
}

TEST_F(ApplicationTest, RendererReceivesWindowDimensions) {
    window->m_width = 1280;
    window->m_height = 720;
    Application app(window, renderer);
    EXPECT_TRUE(app.initialize());
    EXPECT_EQ(renderer->m_width, 1280);
    EXPECT_EQ(renderer->m_height, 720);
}
