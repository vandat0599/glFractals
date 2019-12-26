#pragma once

#include "CloseListener.hpp"
#include "Common.hpp"
#include "KeyListener.hpp"
#include "MouseListener.hpp"
#include "ResolutionChangeListener.hpp"

#include <string>
#include <vector>

namespace glFractals {
class Shader;
class StateController : public KeyListener,
                        public CloseListener,
                        public MouseListener,
                        public ResolutionChangeListener {
public:
    virtual ~StateController() {}

    virtual void update(float delta) = 0;

    virtual auto shouldClose() const -> bool = 0;

    // Returns window state information as strings.
    virtual auto stateStrings() const -> std::vector<std::string> = 0;

    virtual void programShader(Shader& shader) const = 0;

    // Listener functions
    // virtual auto notifyClose() -> bool override;
    // virtual void notifyMouse(float cursorX,
    //                  float cursorY,
    //                  float scrollX,
    //                  float scrollY,
    //                  Event event,
    //                  ButtonState state) override;
    // virtual void notifyEvent(Event event, ButtonState state) override;
    // virtual void notifyResolution(int newWidth, int newHeight) override;
};
} // namespace glFractals