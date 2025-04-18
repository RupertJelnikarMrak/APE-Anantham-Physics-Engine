#pragma once

#include "Core/Rendering/FrameInfo.hpp"
#include "Core/Rendering/GraphicsDevice.hpp"
#include "Core/Rendering/Pipeline.hpp"

// std
#include <memory>

namespace Core::Ecs::Systems
{

class PointLightRenderSystem
{
public:
    PointLightRenderSystem(
        Rendering::GraphicsDevice &device,
        VkRenderPass renderPass,
        VkDescriptorSetLayout globalSetLayout);
    ~PointLightRenderSystem();

    PointLightRenderSystem(const PointLightRenderSystem &) = delete;
    PointLightRenderSystem &operator=(const PointLightRenderSystem &) = delete;

    void update(Rendering::FrameInfo &frameInfo, Rendering::GlobalUbo &ubo);
    void render(Rendering::FrameInfo &frameInfo, Rendering::GlobalUbo &ubo);

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Rendering::GraphicsDevice &_device;

    std::unique_ptr<Rendering::Pipeline> _pipeline;
    VkPipelineLayout _pipelineLayout;
};

} // namespace Core::Ecs::Systems
