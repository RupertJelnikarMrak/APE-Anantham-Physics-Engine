#pragma once

#include "ape_device.hpp"
#include "ape_frame_info.hpp"
#include "ape_game_object.hpp"
#include "ape_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace ape
{

class SimpleRenderSystem
{
public:
    SimpleRenderSystem(ApeDevice &device, VkRenderPass renderPass);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(FrameInfo &frameInfo, std::vector<ApeGameObject> &gameObjects);

private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    ApeDevice &apeDevice;

    std::unique_ptr<ApePipeline> apePipeline;
    VkPipelineLayout pipelineLayout;
};

} // namespace ape
