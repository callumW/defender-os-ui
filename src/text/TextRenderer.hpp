#pragma once

#include "../text/ITextRenderer.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GLES3/gl3.h>

#include <array>
#include <string>
#include <unordered_map>

namespace DefenderOS {

/// Cached data for a single rasterised glyph.
struct GlyphInfo {
    GLuint textureId{0};  ///< GL texture holding the glyph bitmap.
    int width{0};         ///< Bitmap width in pixels.
    int height{0};        ///< Bitmap height in pixels.
    int bearingX{0};      ///< Horizontal bearing (pixels left of the glyph).
    int bearingY{0};      ///< Vertical bearing (pixels above the baseline).
    long advance{0};      ///< Horizontal advance in 1/64 pixels.
};

/// FreeType + OpenGL ES 3.0 text renderer.
///
/// loadFont() pre-rasterises printable ASCII (32–127) into per-glyph GL
/// textures, so renderText() can run without any FreeType calls per frame.
///
/// Requires an active EGL/GLES context before initialize() is called (i.e.
/// after Window::initialize() returns true).
class TextRenderer : public ITextRenderer {
public:
    TextRenderer() = default;
    ~TextRenderer() override;

    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(TextRenderer&&) = delete;

    [[nodiscard]] bool initialize(int width, int height) override;
    [[nodiscard]] bool loadFont(const std::string& fontPath,
                                unsigned int pixelSize) override;
    void renderText(const std::string& text,
                    float x, float y, float scale,
                    float r, float g, float b, float a = 1.0f) override;
    void onResize(int width, int height) override;
    void shutdown() override;
    [[nodiscard]] bool isReady() const override;

private:
    [[nodiscard]] GLuint compileShader(GLenum type, const char* source);
    [[nodiscard]] GLuint createShaderProgram();
    [[nodiscard]] bool setupBuffers();
    [[nodiscard]] bool initFreetype();
    [[nodiscard]] bool loadGlyphs();
    [[nodiscard]] GlyphInfo uploadGlyph();
    void freeGlyphTextures();
    void updateProjection();
    void renderGlyph(const GlyphInfo& glyph, float x, float y, float scale);

    FT_Library m_ftLibrary{nullptr};
    FT_Face m_ftFace{nullptr};
    GLuint m_shaderProgram{0};
    GLuint m_vao{0};
    GLuint m_vbo{0};
    GLint m_projectionLoc{-1};
    GLint m_textColorLoc{-1};
    std::unordered_map<char, GlyphInfo> m_glyphs;
    std::array<float, 16> m_projection{};
    int m_width{0};
    int m_height{0};
    bool m_initialized{false};
};

} // namespace DefenderOS
