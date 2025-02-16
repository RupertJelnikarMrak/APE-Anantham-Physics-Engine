#include "ape_window.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

namespace ape
{

ApeWindow::ApeWindow(int width, int height, std::string name) : width{width}, height{height}, windowName{name}
{
    initWindow();
}

ApeWindow::~ApeWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void ApeWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizedCallback);
}

void ApeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void ApeWindow::framebufferResizedCallback(GLFWwindow *window, int width, int height)
{
    auto apeWindow = reinterpret_cast<ApeWindow *>(glfwGetWindowUserPointer(window));
    apeWindow->framebufferResized = true;
    apeWindow->width = width;
    apeWindow->height = height;
}

} // namespace ape
