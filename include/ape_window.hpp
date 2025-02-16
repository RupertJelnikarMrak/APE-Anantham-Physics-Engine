#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace ape
{
class ApeWindow
{
public:
    ApeWindow(int width, int h, std::string name);
    ~ApeWindow();

    ApeWindow(const ApeWindow &) = delete;
    ApeWindow &operator=(const ApeWindow &) = delete;

    bool shouldClose() { return glfwWindowShouldClose(window); }
    VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
    bool wasWindowResized() { return framebufferResized; }
    void resertWindowResizedFlag() { framebufferResized = false; }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
    static void framebufferResizedCallback(GLFWwindow *window, int width, int height);
    void initWindow();

    int width;
    int height;
    bool framebufferResized = false;

    std::string windowName;
    GLFWwindow *window;
};
} // namespace ape
