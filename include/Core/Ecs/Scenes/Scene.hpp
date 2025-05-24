#pragma once

#include "Core/Ecs/Systems/CameraSystem.hpp"
#include "Core/Ecs/Systems/MeshRenderSystem.hpp"
#include "Core/Input/InputController.hpp"
#include "Core/Rendering/Buffer.hpp"
#include "Core/Rendering/Descriptors.hpp"
#include "Core/Rendering/FrameInfo.hpp"
#include "Core/Rendering/GraphicsDevice.hpp"
#include "Core/Rendering/Renderer.hpp"
#include "Core/Resources/ResourceManager.hpp"

#include <entt/entt.hpp>
#include <memory>
#include <vulkan/vulkan_core.h>

namespace Core::Ecs::Scenes
{

class Scene
{
public:
    Scene(Rendering::GraphicsDevice &device, Rendering::Renderer &renderer, Platform::Window &window);
    ~Scene() = default;

    void drawFrame(float frameTime);

private:
    void createSystems(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    void destroySystems();
    void loadObjects();

    entt::registry _registry;

    Rendering::GlobalUbo _globalUbo{};
    Rendering::GraphicsDevice &_device;
    Rendering::Renderer &_renderer;
    Platform::Window &_window;
    Input::InputController _inputController{_window};
    Resources::ResourceManager _resourceManager{_device};

    std::unique_ptr<Core::Rendering::DescriptorPool> _globalPool{};
    std::vector<std::unique_ptr<Rendering::Buffer>> _uboBuffers{Rendering::SwapChain::MAX_FRAMES_IN_FLIGHT};
    std::vector<VkDescriptorSet> _globalDescriptorSets{Rendering::SwapChain::MAX_FRAMES_IN_FLIGHT};

    Core::Rendering::Camera _camera{};

    std::unique_ptr<Systems::CameraSystem> _cameraSystem;
    std::unique_ptr<Systems::MeshRenderSystem> _meshRenderSystem;
};

} // namespace Core::Ecs::Scenes
