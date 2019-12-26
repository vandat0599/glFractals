#include "FreeTypeWrapper.hpp"
#include "Common.hpp"
#include "Shader.hpp"

#include "gl_utils.h"
#include "glad/glad.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>

namespace glFractals {

static constexpr int MIN_FONT = 12;

FreeTypeWrapper::FreeTypeWrapper(const std::string& fontPath,
                                 float screenRelativeFontHeight,
                                 int resWidth,
                                 int resHeight)
    : screenRelativeFontHeight_(screenRelativeFontHeight), width_(resWidth),
      height_(resHeight)
{
    if (FT_Init_FreeType(&ftLibrary_)) {
        throw std::runtime_error("freetype2 init failed");
    }

    if (FT_New_Face(ftLibrary_, fontPath.c_str(), 0, &ftFace_)) {
        throw std::runtime_error(std::string("freetype2 face failed: ") +
                                 fontPath);
    }

    // NOTE: This assumes no other Renderer changes the OpenGL state.
    GL(glEnable(GL_BLEND));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    loadCharacters(height_ * screenRelativeFontHeight_);

    GL(glGenVertexArrays(1, &vao_));
    GL(glGenBuffers(1, &vbo_));
    GL(glBindVertexArray(vao_));
    GL(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
    GL(glBufferData(
        GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW));
    GL(glEnableVertexAttribArray(0));
    GL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));

    updateOrthoMatrix();
}

void FreeTypeWrapper::renderText(
    Shader& shader, const std::string& text, float x, float y, float scale)
{
    shader.use();

    // TODO: Shaders should have a matrix uniform option.
    GL(int loc = glGetUniformLocation(shader.get(), "projection"));
    // OpenGL is in column major order.
    GL(glUniformMatrix4fv(
        loc, 1, false, reinterpret_cast<float*>(orthoMatrix_)));

    GL(glActiveTexture(GL_TEXTURE0));
    GL(glBindVertexArray(vao_));

    float dimScale = static_cast<float>(height_) / static_cast<float>(width_);
    x *= dimScale; // Prevents the starting point to scale by width.

    for (auto c = text.begin(); c != text.end(); c++) {
        Character ch = characters_[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {{xpos, ypos + h, 0.0, 0.0},
                                {xpos, ypos, 0.0, 1.0},
                                {xpos + w, ypos, 1.0, 1.0},

                                {xpos, ypos + h, 0.0, 0.0},
                                {xpos + w, ypos, 1.0, 1.0},
                                {xpos + w, ypos + h, 1.0, 0.0}};

        GL(glBindTexture(GL_TEXTURE_2D, ch.texID));
        GL(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
        GL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
        GL(glDrawArrays(GL_TRIANGLES, 0, 6));
        x += (static_cast<float>(ch.advance) / 64.0f) * scale;
    }
}

FreeTypeWrapper::~FreeTypeWrapper()
{
    unloadCharacters();

    GL(glDeleteBuffers(1, &vbo_));
    GL(glDeleteVertexArrays(1, &vao_));

    FT_Done_Face(ftFace_);
    FT_Done_FreeType(ftLibrary_);
}

void FreeTypeWrapper::changeResolution(Point2D<int> resolution)
{
    changeResolution(resolution.x, resolution.y);
}

void FreeTypeWrapper::changeResolution(int width, int height)
{
    width_ = width;
    height_ = height;

    unloadCharacters();
    loadCharacters(height_ * screenRelativeFontHeight_);
    updateOrthoMatrix();
}

void FreeTypeWrapper::loadCharacters(int font)
{
    unloadCharacters();

    FT_Set_Pixel_Sizes(ftFace_, 0, std::max(font, MIN_FONT));

    // OpenGL requires textures to be 4byte aligned, but FreeType will use 1byte
    // textures (single channel). Therefore, we need to edit the alignment.
    GL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    characters_.resize(NUM_CHARACTERS);
    for (int c = 0; c < NUM_CHARACTERS; c++) {
        if (FT_Load_Char(ftFace_, c, FT_LOAD_RENDER)) {
            std::stringstream ss;
            ss << "freetype2 failed to load glyph " << c;
            throw std::runtime_error(ss.str());
        }

        GLuint tex;
        GL(glGenTextures(1, &tex));
        GL(glBindTexture(GL_TEXTURE_2D, tex));
        GL(glTexImage2D(GL_TEXTURE_2D,                   // target
                        0,                               // level (LOD)
                        GL_RED,                          // internal format
                        ftFace_->glyph->bitmap.width,    // width
                        ftFace_->glyph->bitmap.rows,     // height
                        0,                               // border (must be 0)
                        GL_RED,                          // format
                        GL_UNSIGNED_BYTE,                // type
                        ftFace_->glyph->bitmap.buffer)); // buf

        GL(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GL(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        characters_[c] = {
            tex,
            Point2D<int>(ftFace_->glyph->bitmap.width,
                         ftFace_->glyph->bitmap.rows),
            Point2D<int>(ftFace_->glyph->bitmap_left,
                         ftFace_->glyph->bitmap_top),
            static_cast<std::uint32_t>(ftFace_->glyph->advance.x)};

        maxBearingY_ = std::max(maxBearingY_,
                                static_cast<float>(ftFace_->glyph->bitmap_top));
    }
}

void FreeTypeWrapper::unloadCharacters()
{
    for (auto& ch: characters_) {
        if (ch.texID != 0) {
            GL(glDeleteTextures(1, &ch.texID));
            ch.texID = 0;
        }
    }
    maxBearingY_ = std::numeric_limits<float>::min();
}

auto FreeTypeWrapper::screenRelativeFontHeight() -> float
{
    return screenRelativeFontHeight_;
}

auto FreeTypeWrapper::maxBearingY() -> float { return maxBearingY_; }

void FreeTypeWrapper::updateOrthoMatrix()
{
    // Setup the ortho matrix.
    const float r = width_;
    const float l = 0;
    const float t = height_;
    const float b = 0;
    const float n = 1.0f;
    const float f = -1.0f;

    orthoMatrix_[0][0] = 2.0 / (r - l);
    orthoMatrix_[0][1] = 0;
    orthoMatrix_[0][2] = 0;
    orthoMatrix_[0][3] = 0;

    orthoMatrix_[1][0] = 0;
    orthoMatrix_[1][1] = 2.0 / (t - b);
    orthoMatrix_[1][2] = 0;
    orthoMatrix_[1][3] = 0;

    orthoMatrix_[2][0] = 0;
    orthoMatrix_[2][1] = 0;
    orthoMatrix_[2][2] = -2.0 / (f - n);
    orthoMatrix_[2][3] = 0;

    orthoMatrix_[3][0] = -(r + l) / (r - l);
    orthoMatrix_[3][1] = -(t + b) / (t - b);
    orthoMatrix_[3][2] = -(f + n) / (f - n);
    orthoMatrix_[3][3] = 1;
}

} // namespace glFractals
