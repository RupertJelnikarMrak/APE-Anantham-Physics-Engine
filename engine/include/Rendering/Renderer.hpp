#pragma once

#include "Platform/Window.hpp"
#include "Rendering/GraphicsDevice.hpp"
#include "Rendering/RenderQueue.hpp"
#include "Rendering/SwapChain.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace Anantham::Rendering
{

class Renderer
{
public:
    Renderer(Platform::Window &window, GraphicsDevice &device);
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return _swapChain->getRenderPass(); }
    float getAspectRatio() const { return _swapChain->extentAspectRatio(); }
    bool isFrameInProgress() const { return _isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(_isFrameStarted && "Cannot get command buffer when frame not in progress");
        return _commandBuffers[_currentFrameIndex];
    }

    int getFrameIndex() const
    {
        assert(_isFrameStarted && "Cannot get frame index when frame not in progress");
        return _currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    Platform::Window &_window;
    GraphicsDevice &_device;
    std::unique_ptr<SwapChain> _swapChain;
    std::vector<VkCommandBuffer> _commandBuffers;

    uint32_t _currentImageIndex;
    int _currentFrameIndex{0};
    bool _isFrameStarted{false};
};

} // namespace Anantham::Rendering
