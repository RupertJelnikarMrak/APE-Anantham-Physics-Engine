#include "Core/Rendering/Renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace Core::Rendering
{

Renderer::Renderer(Window &window, Device &device) : _lveWindow{window}, _lveDevice{device}
{
    recreateSwapChain();
    createCommandBuffers();
}

Renderer::~Renderer() { freeCommandBuffers(); }

void Renderer::recreateSwapChain()
{
    auto extent = _lveWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = _lveWindow.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(_lveDevice.getDevice());

    if (_lveSwapChain == nullptr) {
        _lveSwapChain = std::make_unique<SwapChain>(_lveDevice, extent);
    } else {
        std::shared_ptr<SwapChain> oldSwapChain = std::move(_lveSwapChain);
        _lveSwapChain = std::make_unique<SwapChain>(_lveDevice, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*_lveSwapChain.get())) {
            throw std::runtime_error("Swap chain image(or depth) format has changed!");
        }
    }
}

void Renderer::createCommandBuffers()
{
    _commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = _lveDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

    if (vkAllocateCommandBuffers(_lveDevice.getDevice(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Renderer::freeCommandBuffers()
{
    vkFreeCommandBuffers(
        _lveDevice.getDevice(),
        _lveDevice.getCommandPool(),
        static_cast<uint32_t>(_commandBuffers.size()),
        _commandBuffers.data());
    _commandBuffers.clear();
}

VkCommandBuffer Renderer::beginFrame()
{
    assert(!_isFrameStarted && "Can't call beginFrame while already in progress");

    auto result = _lveSwapChain->acquireNextImage(&_currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    _isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    return commandBuffer;
}

void Renderer::endFrame()
{
    assert(_isFrameStarted && "Can't call endFrame while frame is not in progress");
    auto commandBuffer = getCurrentCommandBuffer();
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    auto result = _lveSwapChain->submitCommandBuffers(&commandBuffer, &_currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _lveWindow.wasResized()) {
        _lveWindow.resetResizedFlag();
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    _isFrameStarted = false;
    _currentFrameIndex = (_currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(_isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(
        commandBuffer == getCurrentCommandBuffer() &&
        "Can't begin render pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _lveSwapChain->getRenderPass();
    renderPassInfo.framebuffer = _lveSwapChain->getFrameBuffer(_currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _lveSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_lveSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(_lveSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, _lveSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(_isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
    assert(
        commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");
    vkCmdEndRenderPass(commandBuffer);
}

} // namespace Core::Rendering
