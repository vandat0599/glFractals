#pragma once

#include "StateController.hpp"

#include <string>
#include <vector>

namespace glFractals {
class MandelbrotController : public StateController {
public:
    MandelbrotController(Point2D<int> resolution);

    void update(float delta) override;
    auto shouldClose() const -> bool override;
    auto stateStrings() const -> std::vector<std::string> override;
    void programShader(Shader& shader) const override;

    // Listener functions
    auto notifyClose() -> bool override;
    void notifyMouse(float cursorX,
                     float cursorY,
                     float scrollX,
                     float scrollY,
                     Event event,
                     ButtonState state) override;
    void notifyEvent(Event event, ButtonState state) override;
    void notifyResolution(int newWidth, int newHeight) override;

    void resetCamera();
    auto iterations() const -> int;
    // Helper to convert a screen coordinate (usually a cursor) to complex
    // coordinates.
    auto screenToComp(Point2D<float> p) const -> Point2D<float>;
    auto compCenter() const -> Point2D<float>;
    auto compResolution() const -> Point2D<float>;
    auto compCursor() const -> Point2D<float>;

private:
    bool shouldClose_ = false;

    Point2D<int> resolution_ = {};
    int iterations_ = 100;

    int keyMoveUp_ = 0;
    int keyMoveDown_ = 0;
    int keyMoveLeft_ = 0;
    int keyMoveRight_ = 0;

    float compHeight_ = 2.5f;
    Point2D<float> compCenter_ = {};

    bool cursorDown_ = false;
    float zoomFactor_ = 1.0f;

    // Note cursors are in screen coordinates. Due to floating point
    // precision, don't save the cursors as complex coordinates.
    Point2D<float> curCursor_ = {};
    Point2D<float> prevCursor_ = {};

    static constexpr float ZOOM_FACTOR = 0.85f;
};
} // namespace glFractals