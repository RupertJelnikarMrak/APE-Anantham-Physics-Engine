#pragma once

#include "Core/Rendering/Device.hpp"
#include "Core/Rendering/FrameInfo.hpp"
#include "Core/Rendering/Pipeline.hpp"

// std
#include <memory>

namespace Core::System
{
class SimpleRenderSystem
{
public:
    SimpleRenderSystem(Rendering::Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(Rendering::FrameInfo &frameInfo);

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Rendering::Device &lveDevice;

    std::unique_ptr<Rendering::Pipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
};
} // namespace Core::System
