#pragma once

namespace DefenderOS {

/// Abstract interface for the GPU renderer.
///
/// Implementations wrap OpenGL ES (or any other backend) behind this
/// boundary, enabling unit testing through mock objects.
class IRenderer {
public:
    virtual ~IRenderer() = default;

    /// Set up the OpenGL ES context for the given viewport dimensions.
    /// Returns false on failure.
    [[nodiscard]] virtual bool initialize(int width, int height) = 0;

    /// Clear buffers and prepare for a new frame.
    virtual void beginFrame() = 0;

    /// Finalise the frame (does not swap buffers — that is IWindow's job).
    virtual void endFrame() = 0;

    /// Release all GPU resources.
    virtual void shutdown() = 0;

    /// Update the viewport after a window resize.
    virtual void onResize(int width, int height) = 0;
};

} // namespace DefenderOS
