#pragma once

#include "ape_device.hpp"
#include "ape_game_object.hpp"
#include "ape_pipeline.hpp"
#include "ape_swap_chain.hpp"
#include "ape_window.hpp"

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ape
{

class FirstApp
{
public:
    static constexpr int WIDTH = 1800;
    static constexpr int HEIGHT = 1350;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

private:
    void loadGameObjects();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);
    void renderGameObjects(VkCommandBuffer commandBuffer);

    ApeWindow apeWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    ApeDevice apeDevice{apeWindow};
    std::unique_ptr<ApeSwapChain> apeSwapChain;
    std::unique_ptr<ApePipeline> apePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<ApeGameObject> gameObjects;
};
} // namespace ape
