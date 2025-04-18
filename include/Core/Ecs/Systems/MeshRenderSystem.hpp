#pragma once

#include "Core/Rendering/FrameInfo.hpp"
#include "Core/Rendering/GraphicsDevice.hpp"
#include "Core/Rendering/Pipeline.hpp"
#include "Core/Resources/ResourceManager.hpp"

#include <entt/entt.hpp>

// std
#include <glm/fwd.hpp>
#include <memory>
#include <vulkan/vulkan_core.h>

namespace Core::Ecs::Systems
{

class MeshRenderSystem
{
public:
    MeshRenderSystem(Rendering::GraphicsDevice &, VkRenderPass, VkDescriptorSetLayout, Resources::ResourceManager &);
    ~MeshRenderSystem();

    MeshRenderSystem(const MeshRenderSystem &) = delete;
    MeshRenderSystem &operator=(const MeshRenderSystem &) = delete;
    MeshRenderSystem(MeshRenderSystem &&) = delete;
    MeshRenderSystem &operator=(MeshRenderSystem &&) = delete;

    void operator()(Rendering::FrameInfo &frameInfo) { render(frameInfo); }

    void render(Rendering::FrameInfo &frameInfo);

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Rendering::GraphicsDevice &_device;

    Resources::ResourceManager &_resourceManager;

    std::unique_ptr<Rendering::Pipeline> _pipeline;
    VkPipelineLayout _pipelineLayout;
};

} // namespace Core::Ecs::Systems
