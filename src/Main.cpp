#include "Common.hpp"
#include "Event.hpp"
#include "FractalRenderer.hpp"
#include "Framework.hpp"
#include "JuliaController.hpp"
#include "MandelbrotController.hpp"
#include "Shader.hpp"
#include "StateController.hpp"
#include "TextRenderer.hpp"

#include "GLFW/glfw3.h"

#include <iostream>
#include <memory>
#include <vector>

enum class FractalType { MANDELBROT, JULIA };

auto buildFractalShader(FractalType type) -> glFractals::Shader
{
    std::vector<glFractals::Shader::Source> sources;
    sources.push_back({glFractals::Shader::Source::Type::VERTEX_SHADER,
                       ROOT_PATH_STR + "/src/shaders/Fractal.vs"});
    if (type == FractalType::MANDELBROT) {
        sources.push_back({glFractals::Shader::Source::Type::FRAGMENT_SHADER,
                           ROOT_PATH_STR + "/src/shaders/Mandelbrot.fs"});
    }
    else if (type == FractalType::JULIA) {
        sources.push_back({glFractals::Shader::Source::Type::FRAGMENT_SHADER,
                           ROOT_PATH_STR + "/src/shaders/Julia.fs"});
    }
    return glFractals::Shader(sources);
}

auto buildTextShader() -> glFractals::Shader
{
    std::vector<glFractals::Shader::Source> sources;
    sources.push_back({glFractals::Shader::Source::Type::VERTEX_SHADER,
                       ROOT_PATH_STR + "/src/shaders/Text.vs"});
    sources.push_back({glFractals::Shader::Source::Type::FRAGMENT_SHADER,
                       ROOT_PATH_STR + "/src/shaders/Text.fs"});

    return glFractals::Shader(sources);
}

auto stateControllerFactory(FractalType fractalType, Point2D<int> resolution)
    -> std::unique_ptr<glFractals::StateController>
{
    if (fractalType == FractalType::MANDELBROT) {
        return std::make_unique<glFractals::MandelbrotController>(resolution);
    }
    else {
        return std::make_unique<glFractals::JuliaController>(resolution);
    }
}

auto main(int argc, char** argv) -> int
{
    auto args = std::vector<std::string>(argv, argv + argc);
    auto fractalType = (args.size() < 2 || args[1] != "julia")
                           ? FractalType::MANDELBROT
                           : FractalType::JULIA;

    auto framework = glFractals::Framework();

    auto controller =
        stateControllerFactory(fractalType, framework.resolution());

    auto fractalShader = buildFractalShader(fractalType);
    auto fractalRenderer = glFractals::FractalRenderer(framework.resolution());

    auto textShader = buildTextShader();
    auto textRenderer = glFractals::TextRenderer(
        textShader,
        ROOT_PATH_STR + "/dep/fonts/amiko/Amiko-Regular.ttf",
        0.025,
        framework.resolution());

    framework.mapButton(GLFW_KEY_ESCAPE, glFractals::Event::EXIT);
    framework.mapButton(GLFW_KEY_W, glFractals::Event::MOVE_UP);
    framework.mapButton(GLFW_KEY_S, glFractals::Event::MOVE_DOWN);
    framework.mapButton(GLFW_KEY_A, glFractals::Event::MOVE_LEFT);
    framework.mapButton(GLFW_KEY_D, glFractals::Event::MOVE_RIGHT);
    framework.mapButton(GLFW_KEY_E, glFractals::Event::INCREASE_ITERATIONS);
    framework.mapButton(GLFW_KEY_Q, glFractals::Event::DECREASE_ITERATIONS);
    framework.mapButton(GLFW_KEY_SPACE, glFractals::Event::RESET_CAMERA);
    framework.mapMouseButton(GLFW_MOUSE_BUTTON_1,
                             glFractals::Event::DRAG_CAMERA);
    framework.mapMouseButton(GLFW_MOUSE_BUTTON_2, glFractals::Event::DRAG_SEED);

    framework.registerCloseListener(*controller);
    framework.registerKeyListener(*controller);
    framework.registerMouseListener(*controller);
    framework.registerResolutionChangeListener(*controller);
    framework.registerResolutionChangeListener(fractalRenderer);
    framework.registerResolutionChangeListener(textRenderer);

    auto prevFrame = framework.time();
    while (!controller->shouldClose()) {
        auto curFrame = framework.time();
        auto delta = curFrame - prevFrame;
        prevFrame = curFrame;

        controller->update(delta);

        fractalRenderer.render(fractalShader, *controller);
        textRenderer.render(controller->stateStrings());

        framework.swapBuffers();
        framework.updateEvents();
    }

    return 0;
}