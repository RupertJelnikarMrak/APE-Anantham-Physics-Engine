#pragma once

#include "Core/Rendering/Descriptors.hpp"
#include "Core/Rendering/Device.hpp"
#include "Core/Rendering/GameObject.hpp"
#include "Core/Rendering/Renderer.hpp"
#include "Core/Window.hpp"

// std
#include <memory>

namespace Core
{
class FirstApp
{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

private:
    void loadGameObjects();

    Window lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
    Rendering::Device lveDevice{lveWindow};
    Rendering::Renderer lveRenderer{lveWindow, lveDevice};

    // note: order of declarations matters
    std::unique_ptr<Rendering::DescriptorPool> globalPool{};
    Rendering::GameObject::Map gameObjects;
};
} // namespace Core
