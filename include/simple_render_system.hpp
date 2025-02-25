#pragma once

#include "ape_device.hpp"
#include "ape_game_object.hpp"
#include "ape_pipeline.hpp"

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ape
{

class SimpleRenderSystem
{
public:
    SimpleRenderSystem(ApeDevice &device, VkRenderPass renderPass);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<ApeGameObject> &gameObjects);

private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    ApeDevice &apeDevice;

    std::unique_ptr<ApePipeline> apePipeline;
    VkPipelineLayout pipelineLayout;
};

} // namespace ape
