#pragma once

namespace glFractals {
enum class Event {
    NONE,
    EXIT,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    DRAG_CAMERA,
    INCREASE_ITERATIONS,
    DECREASE_ITERATIONS,
    RESET_CAMERA,
    DRAG_SEED // Only for Julia fractals
};
}