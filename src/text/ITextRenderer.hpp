#pragma once

#include <string>

namespace DefenderOS {

/// Abstract interface for GPU-accelerated text rendering.
///
/// Implementations use FreeType to rasterise glyphs and OpenGL ES to render
/// them.  The interface is kept narrow so MockTextRenderer can stand in during
/// unit tests without requiring a display or font file.
class ITextRenderer {
public:
    virtual ~ITextRenderer() = default;

    /// Set up GPU resources (shaders, VBO). Must be called while an OpenGL ES
    /// context is active. Returns false on failure.
    [[nodiscard]] virtual bool initialize(int width, int height) = 0;

    /// Load a TrueType/OpenType font file and pre-rasterise printable ASCII
    /// glyphs at @p pixelSize. Returns false if the file cannot be opened.
    [[nodiscard]] virtual bool loadFont(const std::string& fontPath,
                                        unsigned int pixelSize) = 0;

    /// Render @p text with its origin at screen coordinate (@p x, @p y).
    /// @p y is measured from the bottom of the viewport (OpenGL convention).
    /// @p scale multiplies the rasterised glyph size.
    /// Colour components are in linear [0, 1] range.
    virtual void renderText(const std::string& text,
                            float x, float y, float scale,
                            float r, float g, float b, float a = 1.0f) = 0;

    /// Update the projection matrix after a window resize.
    virtual void onResize(int width, int height) = 0;

    /// Release all GPU and FreeType resources.
    virtual void shutdown() = 0;

    /// Returns true once initialize() has succeeded.
    [[nodiscard]] virtual bool isReady() const = 0;
};

} // namespace DefenderOS
