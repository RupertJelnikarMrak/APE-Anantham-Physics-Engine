#include "ape_renderer.hpp"
#include "ape_swap_chain.hpp"

// std
#include <array>
#include <cassert>
#include <memory>
#include <stdexcept>

namespace ape
{

ApeRenderer::ApeRenderer(ApeWindow &window, ApeDevice &device) : apeWindow{window}, apeDevice{device}
{
    recreateSwapChain();
    createCommandBuffers();
}

ApeRenderer::~ApeRenderer() { freeCommandBuffers(); }

void ApeRenderer::recreateSwapChain()
{
    auto extent = apeWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = apeWindow.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(apeDevice.device());

    if (apeSwapChain == nullptr) {
        apeSwapChain = std::make_unique<ApeSwapChain>(apeDevice, extent);
    } else {
        std::shared_ptr<ApeSwapChain> oldSwapChain = std::move(apeSwapChain);
        apeSwapChain = std::make_unique<ApeSwapChain>(apeDevice, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*apeSwapChain.get())) {
            throw std::runtime_error("Swap chain image/depth format has changed!");
        }
    }
}

void ApeRenderer::createCommandBuffers()
{
    commandBuffers.resize(ApeSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = apeDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(apeDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void ApeRenderer::freeCommandBuffers()
{
    vkFreeCommandBuffers(
        apeDevice.device(),
        apeDevice.getCommandPool(),
        static_cast<uint32_t>(commandBuffers.size()),
        commandBuffers.data());
    commandBuffers.clear();
}

VkCommandBuffer ApeRenderer::beginFrame()
{
    assert(!isFrameStarted && "Can't call beginFrame while frame is already in progress.");

    auto result = apeSwapChain->acquireNextImage(&currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire next swap chain image!");
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    return commandBuffer;
}

void ApeRenderer::endFrame()
{
    assert(isFrameStarted && "Can't call endFrame while frame is not in progress.");
    auto commandBuffer = getCurrentCommandBuffer();

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    auto result = apeSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || apeWindow.wasWindowResized()) {
        apeWindow.resertWindowResizedFlag();
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to submit command buffer!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % ApeSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void ApeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(isFrameStarted && "Can't begin render pass when frame is not in progress.");
    assert(
        commandBuffer == getCurrentCommandBuffer() &&
        "Can't begin render pass on command buffer from a different frame.");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = apeSwapChain->getRenderPass();
    renderPassInfo.framebuffer = apeSwapChain->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = apeSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(apeSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(apeSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, apeSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void ApeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(isFrameStarted && "Can't end render pass when frame is not in progress.");
    assert(
        commandBuffer == getCurrentCommandBuffer() &&
        "Can't end render pass on command buffer from a different frame.");

    vkCmdEndRenderPass(commandBuffer);
}

} // namespace ape
