#include <gtest/gtest.h>

#include "../src/core/EventLoop.hpp"
#include "mocks/MockRenderer.hpp"
#include "mocks/MockWindow.hpp"

using namespace DefenderOS;
using namespace DefenderOS::Test;

class EventLoopTest : public ::testing::Test {
protected:
    std::shared_ptr<MockWindow> window;
    std::shared_ptr<MockRenderer> renderer;

    void SetUp() override {
        window = std::make_shared<MockWindow>();
        renderer = std::make_shared<MockRenderer>();
    }
};

TEST_F(EventLoopTest, ThrowsOnNullWindow) {
    EXPECT_THROW(EventLoop(nullptr, renderer), std::invalid_argument);
}

TEST_F(EventLoopTest, ThrowsOnNullRenderer) {
    EXPECT_THROW(EventLoop(window, nullptr), std::invalid_argument);
}

TEST_F(EventLoopTest, ConstructsSuccessfullyWithValidArguments) {
    EXPECT_NO_THROW(EventLoop(window, renderer));
}

TEST_F(EventLoopTest, IsNotRunningBeforeStart) {
    EventLoop loop(window, renderer);
    EXPECT_FALSE(loop.isRunning());
}

TEST_F(EventLoopTest, ExitsImmediatelyWhenWindowShouldClose) {
    window->m_shouldClose = true;
    EventLoop loop(window, renderer);
    loop.run();
    EXPECT_FALSE(loop.isRunning());
}

TEST_F(EventLoopTest, CallsUpdateCallbackEachFrame) {
    window->m_shouldClose = false;
    EventLoop loop(window, renderer);

    int count = 0;
    loop.setUpdateCallback([&]() {
        ++count;
        if (count >= 3) {
            window->m_shouldClose = true;
        }
    });

    loop.run();
    EXPECT_EQ(count, 3);
}

TEST_F(EventLoopTest, CallsRenderCallbackEachFrame) {
    window->m_shouldClose = false;
    EventLoop loop(window, renderer);

    int renderCount = 0;
    loop.setUpdateCallback([&]() { window->m_shouldClose = true; });
    loop.setRenderCallback([&]() { ++renderCount; });

    loop.run();
    EXPECT_GE(renderCount, 1);
}

TEST_F(EventLoopTest, PollsEventsEachFrame) {
    window->m_shouldClose = false;
    EventLoop loop(window, renderer);
    int frames = 0;

    loop.setUpdateCallback([&]() {
        ++frames;
        if (frames >= 5) {
            window->m_shouldClose = true;
        }
    });

    loop.run();
    EXPECT_EQ(window->m_pollCount.load(), frames);
}

TEST_F(EventLoopTest, SwapsBuffersEachFrame) {
    window->m_shouldClose = false;
    EventLoop loop(window, renderer);
    int frames = 0;

    loop.setUpdateCallback([&]() {
        ++frames;
        if (frames >= 2) {
            window->m_shouldClose = true;
        }
    });

    loop.run();
    EXPECT_EQ(window->m_swapCount.load(), frames);
}

TEST_F(EventLoopTest, CallsBeginAndEndFrameEachFrame) {
    window->m_shouldClose = false;
    EventLoop loop(window, renderer);

    loop.setUpdateCallback([&]() { window->m_shouldClose = true; });
    loop.run();

    EXPECT_GE(renderer->m_beginFrameCount.load(), 1);
    EXPECT_GE(renderer->m_endFrameCount.load(), 1);
    EXPECT_EQ(renderer->m_beginFrameCount.load(), renderer->m_endFrameCount.load());
}

TEST_F(EventLoopTest, StopExitsTheLoop) {
    window->m_shouldClose = false;
    EventLoop loop(window, renderer);

    loop.setUpdateCallback([&]() { loop.stop(); });
    loop.run();

    EXPECT_FALSE(loop.isRunning());
}

TEST_F(EventLoopTest, RunIsIdempotentWhenCalledTwice) {
    window->m_shouldClose = true;
    EventLoop loop(window, renderer);
    loop.run();
    // Second call on a closed window should also complete without error.
    EXPECT_NO_THROW(loop.run());
}
