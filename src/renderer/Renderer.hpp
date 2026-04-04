#pragma once

#include "../renderer/IRenderer.hpp"

namespace DefenderOS {

/// OpenGL ES 3.0 renderer.
///
/// Requires an active EGL/GLES context (created by Window) before initialize()
/// is called.  All GLES calls are confined to this class so the rest of the
/// codebase remains GPU-agnostic.
class Renderer : public IRenderer {
public:
    Renderer() = default;
    ~Renderer() override;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    [[nodiscard]] bool initialize(int width, int height) override;
    void beginFrame() override;
    void endFrame() override;
    void shutdown() override;
    void onResize(int width, int height) override;

private:
    int m_width{0};
    int m_height{0};
    bool m_initialized{false};
};

} // namespace DefenderOS
