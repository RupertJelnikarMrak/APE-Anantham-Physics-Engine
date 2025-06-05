#pragma once

#include "Platform/Window.hpp"
#include "Rendering/Descriptors.hpp"
#include "Rendering/GraphicsDevice.hpp"
#include "Rendering/Renderer.hpp"

// std
#include <memory>

namespace Anantham
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
    Platform::Window _window{WIDTH, HEIGHT, "Vulkan Engine"};
    Rendering::GraphicsDevice _device{_window};
    Rendering::Renderer _renderer{_window, _device};

    std::unique_ptr<Rendering::DescriptorPool> _descriptorPool;
};

} // namespace Anantham
