#pragma once

#include "Core/Rendering/Device.hpp"
#include "Core/Rendering/FrameInfo.hpp"
#include "Core/Rendering/Pipeline.hpp"

// std
#include <memory>

namespace Core::System
{

class PointLightSystem
{
public:
    PointLightSystem(Rendering::Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~PointLightSystem();

    PointLightSystem(const PointLightSystem &) = delete;
    PointLightSystem &operator=(const PointLightSystem &) = delete;

    void update(Rendering::FrameInfo &frameInfo, Rendering::GlobalUbo &ubo);
    void render(Rendering::FrameInfo &frameInfo);

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Rendering::Device &lveDevice;

    std::unique_ptr<Rendering::Pipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
};

} // namespace Core::System
