#pragma once

#include "Common.hpp"
#include "ResolutionChangeListener.hpp"

#include <cstdint>

namespace glFractals {
class Shader;
class StateController;
class FractalRenderer : public ResolutionChangeListener {
public:
    FractalRenderer(int newWidth, int newHeight);
    FractalRenderer(Point2D<int> initial_resolution);

    virtual ~FractalRenderer();

    void render(Shader& shader, const StateController& controller);

    // Changes the resolution of the rendering.
    void changeResolution(int newWidth, int newHeight);
    void changeResolution(const Point2D<int> newResolution);

    // Called by the ResolutionChangeListener source.
    void notifyResolution(int newWidth, int newHeight) override;

private:
    int width_ = 0;
    int height_ = 0;

    std::uint32_t vao_ = 0;
    std::uint32_t vbo_ = 0;
};
} // namespace glFractals