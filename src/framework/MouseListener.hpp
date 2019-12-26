#pragma once

#include "ButtonState.hpp"

namespace glFractals {

enum class Event;

class MouseListener {
public:
    virtual void notifyMouse(float cursorX,
                             float cursorY,
                             float wheelX,
                             float wheelY,
                             Event event,
                             ButtonState state) = 0;
    virtual ~MouseListener(){};
};

} // namespace glFractals