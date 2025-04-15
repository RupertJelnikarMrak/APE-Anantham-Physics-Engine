#pragma once

// lib
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <cstdint>
#include <string>

namespace Core
{

class Window
{
public:
    Window(int w, int h, std::string name);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    bool shouldClose() const { return glfwWindowShouldClose(_window); }
    VkExtent2D getExtent() const { return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)}; }
    bool wasResized() const { return _resized; }
    void resetResizedFlag() { _resized = false; }
    GLFWwindow *getGLFWwindow() const { return _window; }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    void initWindow();

    int _width;
    int _height;
    bool _resized;

    std::string _windowName;
    GLFWwindow *_window;
};

} // namespace Core
