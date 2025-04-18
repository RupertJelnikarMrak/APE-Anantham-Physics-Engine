#pragma once

#include "Core/Rendering/Descriptors.hpp"
#include "Core/Rendering/GraphicsDevice.hpp"
#include "Core/Rendering/Renderer.hpp"
#include "Core/Window.hpp"

// std
#include <memory>

namespace Core
{

class Engine
{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    Engine();
    ~Engine();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;
    Engine(Engine &&) = delete;
    Engine &operator=(Engine &&) = delete;

    void run();

private:
    Window _window{WIDTH, HEIGHT, "Vulkan Engine"};
    Rendering::GraphicsDevice _device{_window};
    Rendering::Renderer _renderer{_window, _device};

    std::unique_ptr<Rendering::DescriptorPool> _descriptorPool;
};

} // namespace Core
