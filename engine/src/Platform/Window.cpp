#include "Platform/Window.hpp"
#include "Input/InputController.hpp"

// lib
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

// std
#include <stdexcept>

namespace Anantham::Platform
{

Window::Window(int w, int h, std::string name) : _width(w), _height(h), _windowName(name)
{
    initWindow();
}

Window::~Window()
{
    if (_window && glfwGetWindowUserPointer(_window) == this) {
        glfwSetWindowUserPointer(_window, nullptr);
    }
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::setInputController(std::shared_ptr<Input::InputController> controller)
{
    _inputController = controller;
}

void Window::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(_window, this);

    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
    glfwSetKeyCallback(_window, KeyCallback);
    glfwSetMouseButtonCallback(_window, MouseButtonCallback);
    glfwSetCursorPosCallback(_window, CursorPosCallback);
    glfwSetScrollCallback(_window, ScrollCallback);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void Window::framebufferResizeCallback(GLFWwindow *_window, int width, int height)
{
    auto windowInstance = reinterpret_cast<Window *>(glfwGetWindowUserPointer(_window));
    if (windowInstance) {
        windowInstance->_resized = true;
        windowInstance->_width = width;
        windowInstance->_height = height;
    }
}

void Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto windowInstance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (windowInstance && windowInstance->_inputController) {
        windowInstance->_inputController->handleKeyEvent(key, scancode, action, mods);
    } else {
    }
}

void Window::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    auto windowInstance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (windowInstance && windowInstance->_inputController) {
        windowInstance->_inputController->handleMouseButtonEvent(button, action, mods);
    } else {
        SPDLOG_WARN("MouseButtonCallback: Window or InputController pointer invalid!");
    }
}

void Window::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    auto windowInstance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (windowInstance && windowInstance->_inputController) {
        windowInstance->_inputController->handleCursorPosEvent(xpos, ypos);
    } else {
        SPDLOG_WARN("CursorPosCallback: Window or InputController pointer invalid!");
    }
}

void Window::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    auto windowInstance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (windowInstance && windowInstance->_inputController) {
        windowInstance->_inputController->handleScrollEvent(xoffset, yoffset);
    } else {
        SPDLOG_WARN("ScrollCallback: Window or InputController pointer invalid!");
    }
}

void Window::getCursorPosition(double &x, double &y) const { glfwGetCursorPos(_window, &x, &y); }

void Window::setCursorPosition(double x, double y) { glfwSetCursorPos(_window, x, y); }

void Window::setCursorMode(int mode) { glfwSetInputMode(_window, GLFW_CURSOR, mode); }

void Window::setCursorShape(int shape)
{
    GLFWcursor *cursor = glfwCreateStandardCursor(shape);
    if (cursor) {
        glfwSetCursor(_window, cursor);
        glfwDestroyCursor(cursor);
    }
}

} // namespace Anantham::Platform
