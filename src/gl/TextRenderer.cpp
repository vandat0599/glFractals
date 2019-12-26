#include "TextRenderer.hpp"

#include "Common.hpp"

#include "gl_utils.h"
#include "glad/glad.h"

#include <sstream>
#include <stdexcept>

namespace glFractals {

TextRenderer::TextRenderer(Shader& shader,
                           const std::string& fontPath,
                           float screenRelativeFontHeight,
                           int width,
                           int height)
    : shader_(shader), width_(width), height_(height),
      freeTyper_(fontPath, screenRelativeFontHeight, width, height)
{
}

TextRenderer::~TextRenderer() {}

TextRenderer::TextRenderer(Shader& shader,
                           const std::string& fontPath,
                           float screenRelativeFontHeight,
                           Point2D<int> resolution)
    : TextRenderer(shader,
                   fontPath,
                   screenRelativeFontHeight,
                   resolution.x,
                   resolution.y)
{
}

void TextRenderer::render(const std::vector<std::string> stateStrings)
{
    const float bottomPadding =
        0.1 * height_ * freeTyper_.screenRelativeFontHeight();
    const float leftPadding = bottomPadding;
    const float topPadding = bottomPadding;
    const float maxBearingY = freeTyper_.maxBearingY();

    float textPosY = height_;

    for (const auto& str : stateStrings) {
        textPosY -= (maxBearingY + topPadding + bottomPadding);
        freeTyper_.renderText(shader_, str, leftPadding, textPosY, 1.0f);
    }
}

void TextRenderer::changeResolution(int newWidth, int newHeight)
{
    width_ = newWidth;
    height_ = newHeight;

    GL(glViewport(0, 0, width_, height_));
    freeTyper_.changeResolution(width_, height_);
}

void TextRenderer::changeResolution(const Point2D<int> newResolution)
{
    changeResolution(newResolution.x, newResolution.y);
}

void TextRenderer::notifyResolution(int newWidth, int newHeight)
{
    changeResolution(newWidth, newHeight);
}

} // namespace glFractals