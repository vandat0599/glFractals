#include "JuliaController.hpp"
#include "Event.hpp"
#include "Shader.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sstream>

namespace glFractals {

JuliaController::JuliaController(Point2D<int> resolution)
    : controller_(resolution)
{
}

void JuliaController::update(float delta)
{
    controller_.update(delta);

    if (cursorDown_) {
        const auto deltaPos =
            DRAG_SEED_SENSITIVITY * (controller_.screenToComp(curCursor_) -
                                     controller_.screenToComp(prevCursor_));
        seed_.x += deltaPos.x;
        seed_.y += deltaPos.y;
    }

    prevCursor_ = curCursor_;
}

void JuliaController::notifyEvent(Event event, ButtonState state)
{
    controller_.notifyEvent(event, state);
    if (event == Event::RESET_CAMERA && state == ButtonState::PRESSED) {
        resetCamera();
    }
}

void JuliaController::notifyMouse(float cursorX,
                                  float cursorY,
                                  float scrollX,
                                  float scrollY,
                                  Event event,
                                  ButtonState state)
{
    controller_.notifyMouse(cursorX, cursorY, scrollX, scrollY, event, state);
    if (cursorDown_ == false && event == Event::DRAG_SEED &&
        state == ButtonState::PRESSED) {
        cursorDown_ = true;
    }
    else if (cursorDown_ == true && event == Event::DRAG_SEED &&
             state == ButtonState::RELEASED) {
        cursorDown_ = false;
    }

    curCursor_ = {cursorX, cursorY};
}

void JuliaController::notifyResolution(int newWidth, int newHeight)
{
    controller_.notifyResolution(newWidth, newHeight);
}

auto JuliaController::shouldClose() const -> bool
{
    return controller_.shouldClose();
}

auto JuliaController::notifyClose() -> bool
{
    return controller_.notifyClose();
}

auto JuliaController::stateStrings() const -> std::vector<std::string>
{
    auto stateStrings = controller_.stateStrings();
    std::stringstream ss;
    ss << "seed: " << str(seed_);
    stateStrings.push_back(ss.str());
    return stateStrings;
}

void JuliaController::resetCamera()
{
    controller_.resetCamera();
    cursorDown_ = false;
    curCursor_ = {};
    prevCursor_ = {};
}

void JuliaController::programShader(Shader& shader) const
{
    controller_.programShader(shader);
    shader.setUniform("seedX", seed_.x);
    shader.setUniform("seedY", seed_.y);
}

} // namespace glFractals