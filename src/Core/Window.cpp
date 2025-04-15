#include "Core/Window.hpp"

// lib
#include <GLFW/glfw3.h>

// std
#include <stdexcept>

namespace Core
{

Window::Window(int w, int h, std::string name) : _width(w), _height(h), _windowName(name) { initWindow(); }

Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create _window surface!");
    }
}

void Window::framebufferResizeCallback(GLFWwindow *_window, int width, int height)
{
    auto app = reinterpret_cast<Window *>(glfwGetWindowUserPointer(_window));
    app->_resized = true;
    app->_width = width;
    app->_height = height;
}

} // namespace Core
