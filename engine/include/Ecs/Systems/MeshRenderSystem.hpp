#pragma once

#include "Ecs/Systems/ISystem.hpp"
#include "Rendering/FrameInfo.hpp"
#include "Rendering/GraphicsDevice.hpp"
#include "Rendering/Pipeline.hpp"
#include "Resources/ResourceManager.hpp"

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

// std
#include <glm/fwd.hpp>
#include <memory>
#include <vulkan/vulkan_core.h>

namespace Anantham::Ecs::Systems
{

class MeshRenderSystem : public IRenderSystem
{
public:
    MeshRenderSystem(
        Rendering::GraphicsDevice &,
        VkRenderPass,
        VkDescriptorSetLayout,
        Resources::ResourceManager &);
    ~MeshRenderSystem();

    MeshRenderSystem(const MeshRenderSystem &) = delete;
    MeshRenderSystem &operator=(const MeshRenderSystem &) = delete;
    MeshRenderSystem(MeshRenderSystem &&) = delete;
    MeshRenderSystem &operator=(MeshRenderSystem &&) = delete;

    void render(entt::registry &registry, Rendering::FrameInfo &frameInfo) override;

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Rendering::GraphicsDevice &_device;

    Resources::ResourceManager &_resourceManager;

    std::unique_ptr<Rendering::Pipeline> _pipeline;
    VkPipelineLayout _pipelineLayout;
};

} // namespace Anantham::Ecs::Systems
