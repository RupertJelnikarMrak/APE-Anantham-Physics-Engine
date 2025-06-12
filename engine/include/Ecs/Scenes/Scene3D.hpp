#pragma once

#include "IScene.hpp"
#include "Input/InputController.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/Descriptors.hpp"
#include "Rendering/GraphicsDevice.hpp"
#include "Rendering/Renderer.hpp"

namespace Anantham::Ecs::Scenes
{

class Scene3D : public IScene
{
public:
    Scene3D(
        Rendering::GraphicsDevice &graphicsDevice,
        Rendering::Renderer &renderer,
        Input::InputController &inputController);
    ~Scene3D() override = default;

protected:
    Rendering::GraphicsDevice &_graphicsDevice;
    Rendering::Renderer &_renderer;
    Input::InputController &_inputController;
    Rendering::GlobalUbo _globalUbo{};

    std::unique_ptr<Anantham::Rendering::DescriptorPool> _globalPool{};
    std::vector<std::unique_ptr<Rendering::Buffer>> _uboBuffers{
        Rendering::SwapChain::MAX_FRAMES_IN_FLIGHT};
    std::vector<VkDescriptorSet> _globalDescriptorSets{Rendering::SwapChain::MAX_FRAMES_IN_FLIGHT};

    Rendering::Camera _camera{};
};

} // namespace Anantham::Ecs::Scenes
