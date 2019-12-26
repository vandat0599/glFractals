#pragma once

namespace glFractals {
class CloseListener {
public:
    // Returns true if the Close request has been respected.
    // Returns false otherwise.
    virtual auto notifyClose() -> bool = 0;

    virtual ~CloseListener() {}
};
} // namespace glFractals