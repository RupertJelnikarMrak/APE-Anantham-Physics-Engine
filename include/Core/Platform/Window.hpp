#pragma once

// lib
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

// std
#include <cstdint>
#include <string>

namespace Core::Input
{
class InputController;
}

namespace Core::Platform
{

class Window
{
public:
    Window(int w, int h, std::string name);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(Window &&) = delete;

    bool shouldClose() const { return glfwWindowShouldClose(_window); }
    VkExtent2D getExtent() const { return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)}; }
    bool wasResized() const { return _resized; }
    void resetResizedFlag() { _resized = false; }
    GLFWwindow *getGLFWwindow() const { return _window; }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    void setInputController(Input::InputController *controller);

    void getCursorPosition(double &x, double &y) const;

    void setCursorPosition(double x, double y);
    void setCursorMode(int mode);
    void setCursorShape(int shape);

    friend class Input::InputController;

private:
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    void initWindow();

    int _width;
    int _height;
    bool _resized = false;

    std::string _windowName;
    GLFWwindow *_window;

    Input::InputController *_inputControllerPtr = nullptr;
};

} // namespace Core::Platform
