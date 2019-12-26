// TODO: Create a general controller that both Julia and Mandelbrot uses. Right
// now, JuliaController uses uses a MandelbrotController as a member for code
// reuse. Because of this, there is code duplication in how the mouse events are
// handled in the Mandelbrot and Julia controllers to handle the DRAG_SEED
// event.

#pragma once

#include "MandelbrotController.hpp"
#include "StateController.hpp"

#include <memory>
#include <string>
#include <vector>

namespace glFractals {
class JuliaController : public StateController {
public:
    JuliaController(Point2D<int> resolution);

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

private:
    bool cursorDown_ = false;
    Point2D<float> curCursor_ = {};
    Point2D<float> prevCursor_ = {};
    Point2D<float> seed_ = {};
    MandelbrotController controller_;

    static constexpr auto DRAG_SEED_SENSITIVITY = 0.1f;


};
} // namespace glFractals