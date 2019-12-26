#include "Framework.hpp"

#include "CloseListener.hpp"
#include "Event.hpp"
#include "KeyListener.hpp"
#include "MouseListener.hpp"
#include "ResolutionChangeListener.hpp"

#include "gl_utils.h"

// glad needs to be included before GLFW.
// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <cassert>
#include <iostream>

namespace glFractals {

static constexpr auto GL_VER_MAJOR = 3;
static constexpr auto GL_VER_MINOR = 3;
static constexpr auto PROJECT_NAME = "glFractals";

Framework::Framework(int winWidth, int winHeight)
    : winWidth_(winWidth), winHeight_(winHeight)
{
    if (GLFW_FALSE == glfwInit()) {
        throw std::runtime_error("glfw failed to init");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VER_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VER_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window_ =
        glfwCreateWindow(winWidth, winHeight, PROJECT_NAME, nullptr, nullptr);
    if (window_ == nullptr) {
        throw std::runtime_error("glfw window creation failed");
    }

    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("glad load failed");
    }

    glfwSetWindowUserPointer(window_, static_cast<void*>(this));

    glfwSetKeyCallback(window_, keyCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetCursorPosCallback(window_, mouseCursorCallback);
    glfwSetScrollCallback(window_, mouseScrollCallback);
    glfwSetWindowCloseCallback(window_, closeCallback);
    glfwSetFramebufferSizeCallback(window_, resolutionChangeCallback);

    glfwSwapInterval(1);

    GL(glViewport(0, 0, winWidth, winHeight));
    GL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT));
}

Framework::~Framework()
{
    glfwDestroyWindow(window_);
    window_ = nullptr;
    glfwTerminate();
    winWidth_ = 0;
    winHeight_ = 0;
}

void Framework::registerKeyListener(KeyListener& listener)
{
    keyListeners_.push_back(&listener);
}

void Framework::keyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto framework = static_cast<Framework*>(glfwGetWindowUserPointer(window));
    assert(framework != nullptr);

    const auto state = (action == GLFW_PRESS)
                           ? ButtonState::PRESSED
                           : (action == GLFW_RELEASE) ? ButtonState::RELEASED
                                                      : ButtonState::REPEATED;

    auto find_i = framework->keyMap_.find(key);
    if (find_i != framework->keyMap_.end()) {
        for (auto listener : framework->keyListeners_) {
            listener->notifyEvent(find_i->second, state);
        }
    }
}

void Framework::registerMouseListener(MouseListener& listener)
{
    mouseListeners_.push_back(&listener);
}

void Framework::mouseCursorCallback(GLFWwindow* window,
                                    double xpos,
                                    double ypos)
{
    auto framework = static_cast<Framework*>(glfwGetWindowUserPointer(window));
    assert(framework != nullptr);

    for (auto listener : framework->mouseListeners_) {
        listener->notifyMouse(
            xpos, ypos, 0, 0, Event::NONE, ButtonState::RELEASED);
    }
}

void Framework::mouseButtonCallback(GLFWwindow* window,
                                    int key,
                                    int action,
                                    int mods)
{
    auto framework = static_cast<Framework*>(glfwGetWindowUserPointer(window));
    assert(framework != nullptr);

    const auto state = (action == GLFW_PRESS)
                           ? ButtonState::PRESSED
                           : (action == GLFW_RELEASE) ? ButtonState::RELEASED
                                                      : ButtonState::REPEATED;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    auto find_i = framework->mouseMap_.find(key);
    if (find_i != framework->mouseMap_.end()) {
        for (auto listener : framework->mouseListeners_) {
            listener->notifyMouse(xpos, ypos, 0, 0, find_i->second, state);
        }
    }
}

void Framework::mouseScrollCallback(GLFWwindow* window,
                                    double xscroll,
                                    double yscroll)
{
    auto framework = static_cast<Framework*>(glfwGetWindowUserPointer(window));
    assert(framework != nullptr);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    for (auto listener : framework->mouseListeners_) {
        listener->notifyMouse(
            xpos, ypos, xscroll, yscroll, Event::NONE, ButtonState::RELEASED);
        ;
    }
}

void Framework::registerCloseListener(CloseListener& listener)
{
    closeListeners_.push_back(&listener);
}

void Framework::closeCallback(GLFWwindow* window)
{
    auto framework = static_cast<Framework*>(glfwGetWindowUserPointer(window));
    assert(framework != nullptr);

    for (auto listener : framework->closeListeners_) {
        auto request = listener->notifyClose();
        if (request == false) {
            break;
        }
    }
}

void Framework::registerResolutionChangeListener(
    ResolutionChangeListener& listener)
{
    resChangeListeners_.push_back(&listener);
}

void Framework::resolutionChangeCallback(GLFWwindow* window,
                                         int width,
                                         int height)
{
    auto framework = static_cast<Framework*>(glfwGetWindowUserPointer(window));
    assert(framework != nullptr);

    for (auto resObs : framework->resChangeListeners_) {
        resObs->notifyResolution(width, height);
    }
}

void Framework::mapButton(int platformKey, Event event)
{
    keyMap_.insert({platformKey, event});
}

void Framework::mapMouseButton(int platformButton, Event event)
{
    mouseMap_.insert({platformButton, event});
}

void Framework::updateEvents() { glfwPollEvents(); }

void Framework::swapBuffers()
{
    glfwSwapBuffers(window_);
    GL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT));
}

auto Framework::resolution() const -> Point2D<int>
{
    return {winWidth_, winHeight_};
}

auto Framework::time() const -> float { return glfwGetTime(); }

} // namespace glFractals