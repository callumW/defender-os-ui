#include "TextRenderer.hpp"

#include "../logging/Logger.hpp"

#include <GLES3/gl3.h>

namespace DefenderOS {

// ---------------------------------------------------------------------------
// GLSL ES 3.0 shaders
// ---------------------------------------------------------------------------

static constexpr const char* VERTEX_SHADER_SRC = R"(
#version 300 es
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

static constexpr const char* FRAGMENT_SHADER_SRC = R"(
#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 fragColor;
uniform sampler2D text;
uniform vec4 textColor;
void main() {
    float alpha = texture(text, TexCoords).r;
    fragColor = vec4(textColor.rgb, textColor.a * alpha);
}
)";

// ---------------------------------------------------------------------------
// Helper: column-major orthographic projection matrix
// Maps screen coordinates (origin bottom-left) to clip space.
// ---------------------------------------------------------------------------

namespace {

std::array<float, 16> makeOrthoProjection(float w, float h) {
    return {{
        2.0f / w,  0.0f,      0.0f,  0.0f,  // column 0
        0.0f,      2.0f / h,  0.0f,  0.0f,  // column 1
        0.0f,      0.0f,     -1.0f,  0.0f,  // column 2
       -1.0f,     -1.0f,      0.0f,  1.0f   // column 3
    }};
}

} // namespace

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

TextRenderer::~TextRenderer() {
    shutdown();
}

bool TextRenderer::initialize(int width, int height) {
    if (m_initialized) {
        LOG_WARNING("TextRenderer::initialize() called more than once");
        return true;
    }

    m_width = width;
    m_height = height;

    m_shaderProgram = createShaderProgram();
    if (m_shaderProgram == 0) {
        return false;
    }

    m_projectionLoc = glGetUniformLocation(m_shaderProgram, "projection");
    m_textColorLoc  = glGetUniformLocation(m_shaderProgram, "textColor");

    if (!setupBuffers()) {
        return false;
    }

    updateProjection();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_initialized = true;
    LOG_INFO("TextRenderer initialised [" +
             std::to_string(width) + "x" + std::to_string(height) + "]");
    return true;
}

bool TextRenderer::loadFont(const std::string& fontPath, unsigned int pixelSize) {
    if (!m_initialized) {
        LOG_ERROR("TextRenderer::loadFont() called before initialize()");
        return false;
    }

    if (!initFreetype()) {
        return false;
    }

    if (FT_New_Face(m_ftLibrary, fontPath.c_str(), 0, &m_ftFace) != 0) {
        LOG_ERROR("FreeType: failed to load font: " + fontPath);
        return false;
    }

    FT_Set_Pixel_Sizes(m_ftFace, 0, pixelSize);
    freeGlyphTextures();

    if (!loadGlyphs()) {
        LOG_ERROR("TextRenderer: failed to rasterise glyphs for " + fontPath);
        return false;
    }

    LOG_INFO("TextRenderer: loaded font '" + fontPath + "' at " +
             std::to_string(pixelSize) + "px (" +
             std::to_string(m_glyphs.size()) + " glyphs)");
    return true;
}

void TextRenderer::renderText(const std::string& text,
                              float x, float y, float scale,
                              float r, float g, float b, float a) {
    if (!m_initialized || m_glyphs.empty()) {
        return;
    }

    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, m_projection.data());
    glUniform4f(m_textColorLoc, r, g, b, a);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    for (const char ch : text) {
        const auto it = m_glyphs.find(ch);
        if (it == m_glyphs.end()) {
            continue;
        }
        const GlyphInfo& glyph = it->second;
        renderGlyph(glyph, x, y, scale);
        x += static_cast<float>(glyph.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::onResize(int width, int height) {
    m_width = width;
    m_height = height;
    updateProjection();
    LOG_DEBUG("TextRenderer projection updated to " +
              std::to_string(width) + "x" + std::to_string(height));
}

void TextRenderer::shutdown() {
    freeGlyphTextures();

    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_shaderProgram != 0) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
    if (m_ftFace) {
        FT_Done_Face(m_ftFace);
        m_ftFace = nullptr;
    }
    if (m_ftLibrary) {
        FT_Done_FreeType(m_ftLibrary);
        m_ftLibrary = nullptr;
    }

    m_initialized = false;
    LOG_INFO("TextRenderer shut down");
}

bool TextRenderer::isReady() const {
    return m_initialized;
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

GLuint TextRenderer::compileShader(GLenum type, const char* source) {
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string log(static_cast<std::size_t>(len), '\0');
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        LOG_ERROR("Shader compile error: " + log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint TextRenderer::createShaderProgram() {
    const GLuint vert = compileShader(GL_VERTEX_SHADER,   VERTEX_SHADER_SRC);
    const GLuint frag = compileShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);

    if (vert == 0 || frag == 0) {
        glDeleteShader(vert);
        glDeleteShader(frag);
        return 0;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);

    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::string log(static_cast<std::size_t>(len), '\0');
        glGetProgramInfoLog(program, len, nullptr, log.data());
        LOG_ERROR("Shader link error: " + log);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

bool TextRenderer::setupBuffers() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Each quad = 6 vertices * 4 floats (x, y, u, v); updated per character.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}

bool TextRenderer::initFreetype() {
    if (m_ftLibrary) {
        return true;
    }
    if (FT_Init_FreeType(&m_ftLibrary) != 0) {
        LOG_ERROR("Failed to initialise FreeType library");
        return false;
    }
    return true;
}

bool TextRenderer::loadGlyphs() {
    constexpr unsigned char ASCII_PRINTABLE_START = 32;
    constexpr unsigned char ASCII_PRINTABLE_END   = 128;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = ASCII_PRINTABLE_START; c < ASCII_PRINTABLE_END; ++c) {
        if (FT_Load_Char(m_ftFace, c, FT_LOAD_RENDER) != 0) {
            LOG_WARNING("FreeType: could not load glyph for char " +
                        std::to_string(static_cast<int>(c)));
            continue;
        }
        m_glyphs[static_cast<char>(c)] = uploadCurrentGlyph();
    }
    return !m_glyphs.empty();
}

GlyphInfo TextRenderer::uploadCurrentGlyph() {
    const FT_GlyphSlot slot   = m_ftFace->glyph;
    const FT_Bitmap&   bitmap = slot->bitmap;

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                 static_cast<GLsizei>(bitmap.width),
                 static_cast<GLsizei>(bitmap.rows),
                 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return GlyphInfo{
        textureId,
        static_cast<int>(bitmap.width),
        static_cast<int>(bitmap.rows),
        slot->bitmap_left,
        slot->bitmap_top,
        slot->advance.x
    };
}

void TextRenderer::freeGlyphTextures() {
    for (auto& [ch, glyph] : m_glyphs) {
        if (glyph.textureId != 0) {
            glDeleteTextures(1, &glyph.textureId);
            glyph.textureId = 0;
        }
    }
    m_glyphs.clear();
}

void TextRenderer::updateProjection() {
    m_projection = makeOrthoProjection(
        static_cast<float>(m_width),
        static_cast<float>(m_height));
}

void TextRenderer::renderGlyph(const GlyphInfo& glyph, float x, float y, float scale) {
    const float xpos = x + static_cast<float>(glyph.bearingX) * scale;
    const float ypos = y - static_cast<float>(glyph.height - glyph.bearingY) * scale;
    const float w    = static_cast<float>(glyph.width)  * scale;
    const float h    = static_cast<float>(glyph.height) * scale;

    // 6 vertices, each (x, y, u, v). v is inverted to compensate for
    // FreeType's top-down bitmap order versus OpenGL's bottom-up textures.
    const float vertices[6][4] = {
        {xpos,     ypos + h, 0.0f, 0.0f},
        {xpos,     ypos,     0.0f, 1.0f},
        {xpos + w, ypos,     1.0f, 1.0f},
        {xpos,     ypos + h, 0.0f, 0.0f},
        {xpos + w, ypos,     1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}
    };

    glBindTexture(GL_TEXTURE_2D, glyph.textureId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

} // namespace DefenderOS
