#include "MandelbrotController.hpp"
#include "Event.hpp"
#include "Shader.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace glFractals {

static bool pressedOrRepeated(ButtonState state)
{
    return state == ButtonState::PRESSED || state == ButtonState::REPEATED;
}

MandelbrotController::MandelbrotController(Point2D<int> resolution)
    : resolution_(resolution)
{
}

void MandelbrotController::update(float delta)
{
    // When zooming, we want to keep the point under the mouse the same.
    if (zoomFactor_ != 1.0f) {
        auto factor = (zoomFactor_ > 1.0f) ? -(1.0f / ZOOM_FACTOR - 1.0f)
                                           : (1.0f - ZOOM_FACTOR);
        auto deltaCursor = screenToComp(curCursor_) - compCenter_;
        compCenter_ = compCenter_ + factor * deltaCursor;
    }
    compHeight_ *= zoomFactor_;

    if (cursorDown_) {
        auto deltaPos = screenToComp(curCursor_) - screenToComp(prevCursor_);
        compCenter_.x -= deltaPos.x;
        compCenter_.y -= deltaPos.y;
    }

    // Move the center according to WASD.
    auto speed = compHeight_ * delta;
    compCenter_.x =
        compCenter_.x + keyMoveRight_ * speed - keyMoveLeft_ * speed;
    compCenter_.y = compCenter_.y + keyMoveUp_ * speed - keyMoveDown_ * speed;

    zoomFactor_ = 1.0f;
    prevCursor_ = curCursor_;
}

void MandelbrotController::notifyEvent(Event event, ButtonState state)
{
    switch (event) {
        case Event::EXIT:
            shouldClose_ = true;
            break;
        case Event::MOVE_UP:
            keyMoveUp_ = pressedOrRepeated(state);
            break;
        case Event::MOVE_DOWN:
            keyMoveDown_ = pressedOrRepeated(state);
            break;
        case Event::MOVE_LEFT:
            keyMoveLeft_ = pressedOrRepeated(state);
            break;
        case Event::MOVE_RIGHT:
            keyMoveRight_ = pressedOrRepeated(state);
            break;
        case Event::INCREASE_ITERATIONS:
            if (pressedOrRepeated(state)) {
                iterations_ += 1;
            }
            break;
        case Event::DECREASE_ITERATIONS:
            if (pressedOrRepeated(state)) {
                iterations_ = std::max(0, iterations_ - 1);
            }
            break;
        case Event::RESET_CAMERA:
            if (state == ButtonState::PRESSED) {
                resetCamera();
            }
        default:
            break;
    }
}

void MandelbrotController::notifyMouse(float cursorX,
                                       float cursorY,
                                       float scrollX,
                                       float scrollY,
                                       Event event,
                                       ButtonState state)
{
    if (cursorDown_ == false && event == Event::DRAG_CAMERA &&
        state == ButtonState::PRESSED) {
        cursorDown_ = true;
    }
    else if (cursorDown_ == true && event == Event::DRAG_CAMERA &&
             state == ButtonState::RELEASED) {
        cursorDown_ = false;
    }

    curCursor_ = {cursorX, cursorY};

    if (scrollY > 0) {
        zoomFactor_ = ZOOM_FACTOR;
    }
    else if (scrollY < 0) {
        zoomFactor_ = 1.0 / ZOOM_FACTOR;
    }
}

auto MandelbrotController::screenToComp(Point2D<float> p) const
    -> Point2D<float>
{
    const auto dimScale =
        static_cast<float>(resolution_.x) / static_cast<float>(resolution_.y);
    p.x = dimScale * compHeight_ * (p.x - resolution_.x / 2) / resolution_.x +
          compCenter_.x;

    // y is negated because (0,0) is the top left.
    p.y = -1.0 * compHeight_ * (p.y - resolution_.y / 2) / resolution_.y +
          compCenter_.y;
    return p;
}

void MandelbrotController::resetCamera()
{
    iterations_ = 100;

    keyMoveUp_ = 0;
    keyMoveDown_ = 0;
    keyMoveLeft_ = 0;
    keyMoveRight_ = 0;

    compHeight_ = 2.5f;
    compCenter_ = {};

    zoomFactor_ = 1.0f;

    cursorDown_ = false;
    curCursor_ = prevCursor_;
}

auto MandelbrotController::stateStrings() const -> std::vector<std::string>
{
    std::vector<std::string> strs;

    auto cursorPos = compCursor();
    std::stringstream ss;
    ss << "mouse: " << str(cursorPos);
    strs.push_back(ss.str());

    ss.str("");
    auto center = compCenter();
    ss << "center: " << str(center);
    strs.push_back(ss.str());

    ss.str("");
    ss << "iterations: " << iterations();
    strs.push_back(ss.str());

    return strs;
}

void MandelbrotController::programShader(Shader& shader) const
{
    shader.setUniform("iterations", iterations());

    auto res = compResolution();
    shader.setUniform("compWidth", static_cast<float>(res.x));
    shader.setUniform("compHeight", static_cast<float>(res.y));

    auto center = compCenter();
    shader.setUniform("compCenterX", center.x);
    shader.setUniform("compCenterY", center.y);

    shader.setUniform("viewWidth", static_cast<float>(resolution_.x));
    shader.setUniform("viewHeight", static_cast<float>(resolution_.y));
}

void MandelbrotController::notifyResolution(int newWidth, int newHeight)
{
    resolution_ = {newWidth, newHeight};
}

auto MandelbrotController::shouldClose() const -> bool { return shouldClose_; }

auto MandelbrotController::notifyClose() -> bool { return shouldClose_ = true; }

auto MandelbrotController::compCenter() const -> Point2D<float>
{
    return compCenter_;
}

auto MandelbrotController::compResolution() const -> Point2D<float>
{
    const auto dimScale =
        static_cast<float>(resolution_.x) / static_cast<float>(resolution_.y);
    return {dimScale * compHeight_, compHeight_};
}

auto MandelbrotController::compCursor() const -> Point2D<float>
{
    return screenToComp(curCursor_);
}

auto MandelbrotController::iterations() const -> int { return iterations_; }

} // namespace glFractals