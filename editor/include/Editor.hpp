#pragma once

#include "Ecs/Scenes/IScene.hpp"
#include "Input/InputController.hpp"
#include "Platform/Window.hpp"
#include "Rendering/Descriptors.hpp"
#include "Rendering/GraphicsDevice.hpp"
#include "Rendering/Renderer.hpp"
#include "Resources/ResourceManager.hpp"

#include <chrono>
#include <memory>

namespace Editor
{
#define EDITOR_WINDOW_WIDTH 1920
#define EDITOR_WINDOW_HEIGHT 1080
#define EDITOR_WINDOW_NAME "Anantham Editor"

class Editor
{
public:
    Editor();
    ~Editor() = default;

    void run();

private:
    void init();
    void update(float deltaTime);
    void render();
    void stop();

    Anantham::Platform::Window _window{
        EDITOR_WINDOW_WIDTH,
        EDITOR_WINDOW_HEIGHT,
        EDITOR_WINDOW_NAME};
    Anantham::Rendering::GraphicsDevice _graphicsDevice{_window};
    Anantham::Rendering::Renderer _renderer{_window, _graphicsDevice};
    Anantham::Resources::ResourceManager _resourceManager{_graphicsDevice};

    std::shared_ptr<Anantham::Input::InputController> _inputController;
    std::unique_ptr<Anantham::Rendering::DescriptorPool> _imguiPool;
    std::unique_ptr<Anantham::Ecs::Scenes::IScene> _activeScene;

    std::chrono::time_point<std::chrono::steady_clock> _lastFrameTime;
};

} // namespace Editor
