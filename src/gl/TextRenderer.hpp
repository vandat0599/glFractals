#pragma once

#include "Common.hpp"
#include "FreeTypeWrapper.hpp"
#include "ResolutionChangeListener.hpp"

#include <string>

namespace glFractals {
class Shader;
class TextRenderer : public ResolutionChangeListener {
public:
    TextRenderer(Shader& shader,
                 const std::string& fontPath,
                 float screenRelativeFontHeight,
                 int newWidth,
                 int newHeight);
    TextRenderer(Shader& shader,
                 const std::string& fontPath,
                 float screenRelativeFontHeight,
                 Point2D<int> initial_resolution);

    virtual ~TextRenderer();

    void render(const std::vector<std::string> stateStrings);

    // Changes the resolution of the rendering.
    void changeResolution(int newWidth, int newHeight);
    void changeResolution(const Point2D<int> newResolution);

    // Called by the ResolutionChangeListener source.
    void notifyResolution(int newWidth, int newHeight) override;

private:
    Shader& shader_;

    int width_ = 0;
    int height_ = 0;

    FreeTypeWrapper freeTyper_;
};
} // namespace glFractals