#pragma once

#include "ButtonState.hpp"

namespace glFractals {

enum class Event;

class KeyListener {
public:
    virtual void notifyEvent(Event event, ButtonState state) = 0;
    virtual ~KeyListener() {}
};

} // namespace glFractals