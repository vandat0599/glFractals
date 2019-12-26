#pragma once

namespace glFractals {

class ResolutionChangeListener {
public:
    virtual void notifyResolution(int newWidth, int newHeight) = 0;
    virtual ~ResolutionChangeListener() {}
};

} // namespace glFractals