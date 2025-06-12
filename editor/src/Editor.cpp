#include "Editor.hpp"

#include "Platform/Window.hpp"
#include "Rendering/Descriptors.hpp"

#include <chrono>
#include <memory>

namespace Editor
{

Editor::Editor() { init(); }

void Editor::init()
{
    _inputController = _window.getInputController();
    _imguiPool = Anantham::Rendering::DescriptorPool::Builder(_graphicsDevice).build();
}

void Editor::stop() {}

void Editor::run()
{
    _lastFrameTime = std::chrono::steady_clock::now();

    while (!_window.shouldClose()) {
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime =
            std::chrono::duration<float, std::chrono::seconds::period>(currentTime - _lastFrameTime)
                .count();

        update(deltaTime);
        render();
        _window.resetResizedFlag();
    }

    stop();
}

void Editor::update(float deltaTime)
{
    _window.pollEvents();
    _inputController->update();
}

void Editor::render()
{
    VkCommandBuffer commandBuffer = _renderer.beginFrame();
    if (!commandBuffer) {
        return; // Frame was not ready / skipped
    }
}

} // namespace Editor
