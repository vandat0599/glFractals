#pragma once

#include "Common.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstdint>
#include <limits>
#include <memory>
#include <numeric>
#include <vector>

namespace glFractals {
class Shader;
class FreeTypeWrapper {
public:
    FreeTypeWrapper(const std::string& fontPath,
                    float screenRelativeFontHeight,
                    int resWidth,
                    int resHeight);
    ~FreeTypeWrapper();

    void renderText(
        Shader& shader, const std::string& text, float x, float y, float scale);

    void changeResolution(Point2D<int> resolution);
    void changeResolution(int width, int height);

    auto screenRelativeFontHeight() -> float;
    // The bearing is the distance from the text's baseline to the top pixel
    // in pixels.
    auto maxBearingY() -> float;

private:
    std::uint32_t vao_ = 0;
    std::uint32_t vbo_ = 0;

    FT_Library ftLibrary_ = nullptr;
    FT_Face ftFace_ = nullptr;

    float maxBearingY_ = std::numeric_limits<float>::min();

    float screenRelativeFontHeight_;
    int width_;
    int height_;
    float orthoMatrix_[4][4] = {};

    struct Character {
        std::uint32_t texID = 0;
        Point2D<int> size = {};
        Point2D<int> bearing = {};
        std::uint32_t advance = 0;
    };

    static constexpr int NUM_CHARACTERS = 256;
    std::vector<Character> characters_;

    void loadCharacters(int font);
    void unloadCharacters();

    void updateOrthoMatrix();
};

} // namespace glFractals
