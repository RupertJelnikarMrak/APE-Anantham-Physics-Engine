#pragma once

#include "ape_device.hpp"
#include "ape_swap_chain.hpp"
#include "ape_window.hpp"

#include <vulkan/vulkan_core.h>

// std
#include <cassert>
#include <memory>
#include <vector>

namespace ape
{

class ApeRenderer
{
public:
    ApeRenderer(ApeWindow &window, ApeDevice &device);
    ~ApeRenderer();

    ApeRenderer(const ApeRenderer &) = delete;
    ApeRenderer &operator=(const ApeRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return apeSwapChain->getRenderPass(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress.");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const
    {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress.");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    ApeWindow &apeWindow;
    ApeDevice &apeDevice;
    std::unique_ptr<ApeSwapChain> apeSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex = 0;
    bool isFrameStarted = false;
};

} // namespace ape
