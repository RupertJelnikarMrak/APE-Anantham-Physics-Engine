#include "App/App.hpp"

#include "Core/Rendering/Descriptors.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <cassert>
#include <chrono>

namespace App
{

App::App()
{
    _globalPool = Core::Rendering::DescriptorPool::Builder(_device)
                      .setMaxSets(Core::Rendering::SwapChain::MAX_FRAMES_IN_FLIGHT)
                      .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Core::Rendering::SwapChain::MAX_FRAMES_IN_FLIGHT)
                      .build();
    _scene = std::make_unique<Core::Ecs::Scenes::Scene>(_device, _renderer, _window);
}

void App::run()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!_window.shouldClose()) {
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        _scene->drawFrame(frameTime);
    }

    vkDeviceWaitIdle(_device.getDevice());
}

} // namespace App
