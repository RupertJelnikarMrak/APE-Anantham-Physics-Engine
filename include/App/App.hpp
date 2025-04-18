#pragma once

#include "Core/Ecs/Scenes/Scene.hpp"
#include "Core/Platform/Window.hpp"
#include "Core/Rendering/Camera.hpp"
#include "Core/Rendering/Descriptors.hpp"
#include "Core/Rendering/GraphicsDevice.hpp"

#include <memory>

namespace App
{

class App
{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    App();
    ~App() = default;

    App(const App &) = delete;
    App &operator=(const App &) = delete;
    App(App &&) = delete;
    App &operator=(App &&) = delete;

    void run();

private:
    Core::Platform::Window _window{WIDTH, HEIGHT, "Anantham Physics Engine"};
    Core::Rendering::GraphicsDevice _device{_window};
    Core::Rendering::Renderer _renderer{_window, _device};

    std::unique_ptr<Core::Rendering::DescriptorPool> _globalPool{};

    Core::Rendering::Camera _camera{};
    std::unique_ptr<Core::Ecs::Scenes::Scene> _scene;
};

} // namespace App
