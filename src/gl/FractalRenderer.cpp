#include "FractalRenderer.hpp"

#include "Shader.hpp"
#include "StateController.hpp"
#include "gl_utils.h"

#include "glad/glad.h"

namespace glFractals {

// We just need a fullscreen quad to get fragments everywhere on the screen.
// Draw using triangle strip.
static constexpr float QUAD_VERTICES[] = {
    -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
static constexpr int NUM_QUAD_VERTICES =
    sizeof(QUAD_VERTICES) / sizeof(QUAD_VERTICES[0]);

FractalRenderer::FractalRenderer(int newWidth, int newHeight)
    : width_(newWidth), height_(newHeight)
{
    GL(glGenVertexArrays(1, &vao_));
    GL(glGenBuffers(1, &vbo_));

    GL(glBindVertexArray(vao_));
    GL(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
    // target
    // size
    // data
    // usage
    GL(glBufferData(
        GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW));

    // index
    // size
    // type
    // normalized
    // stride
    // pointer offset
    GL(glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
    GL(glEnableVertexAttribArray(0));
}

FractalRenderer::FractalRenderer(Point2D<int> newResolution)
    : FractalRenderer(newResolution.x, newResolution.y)
{
}

FractalRenderer::~FractalRenderer()
{
    GL(glDeleteBuffers(1, &vbo_));
    GL(glDeleteVertexArrays(1, &vao_));
}

void FractalRenderer::render(Shader& shader, const StateController& controller)
{
    shader.use();

    GL(glViewport(0, 0, width_, height_));

    controller.programShader(shader);

    GL(glBindVertexArray(vao_));
    GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, NUM_QUAD_VERTICES));
}

void FractalRenderer::changeResolution(int newWidth, int newHeight)
{
    width_ = newWidth;
    height_ = newHeight;
}

void FractalRenderer::changeResolution(const Point2D<int> newResolution)
{
    changeResolution(newResolution.x, newResolution.y);
}

void FractalRenderer::notifyResolution(int newWidth, int newHeight)
{
    changeResolution(newWidth, newHeight);
}

} // namespace glFractals