#pragma once

#include "Common.hpp"

#include <unordered_map>
#include <vector>

struct GLFWwindow;

namespace glFractals {

enum class Event;
class KeyListener;
class CloseListener;
class MouseListener;
class ResolutionChangeListener;

class Framework {
public:
    static constexpr auto DEFAULT_WIN_WIDTH = 800;
    static constexpr auto DEFAULT_WIN_HEIGHT = 600;

    // Loads the Framework.
    Framework(int winWidth = DEFAULT_WIN_WIDTH,
              int winHeight = DEFAULT_WIN_HEIGHT);
    ~Framework();

    void mapButton(int platformKey, Event event);
    void mapMouseButton(int platformButton, Event event);

    void registerKeyListener(KeyListener& listener);
    void registerCloseListener(CloseListener& listener);
    void registerMouseListener(MouseListener& listener);
    void registerResolutionChangeListener(ResolutionChangeListener& listener);

    // Notifies all listeners.
    void updateEvents();

    void swapBuffers();

    auto resolution() const -> Point2D<int>;

    auto time() const -> float;

private:
    int winWidth_ = 0;
    int winHeight_ = 0;
    GLFWwindow* window_ = nullptr;

    std::unordered_map<int, Event> keyMap_;
    std::unordered_map<int, Event> mouseMap_;

    std::vector<KeyListener*> keyListeners_;
    std::vector<CloseListener*> closeListeners_;
    std::vector<MouseListener*> mouseListeners_;
    std::vector<ResolutionChangeListener*> resChangeListeners_;

    static void keyCallback(
        GLFWwindow* window, int key, int scancode, int action, int mods);
    static void
    mouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
    static void
    mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);
    static void
    mouseScrollCallback(GLFWwindow* window, double xscroll, double yscroll);
    static void closeCallback(GLFWwindow* window);
    static void
    resolutionChangeCallback(GLFWwindow* window, int width, int height);
};
} // namespace glFractals