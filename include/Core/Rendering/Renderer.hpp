#pragma once

#include "Core/Platform/Window.hpp"
#include "Core/Rendering/GraphicsDevice.hpp"
#include "Core/Rendering/SwapChain.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace Core::Rendering
{

class Renderer
{
public:
    Renderer(Platform::Window &window, GraphicsDevice &device);
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return _lveSwapChain->getRenderPass(); }
    float getAspectRatio() const { return _lveSwapChain->extentAspectRatio(); }
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

    Platform::Window &_lveWindow;
    GraphicsDevice &_lveDevice;
    std::unique_ptr<SwapChain> _lveSwapChain;
    std::vector<VkCommandBuffer> _commandBuffers;

    uint32_t _currentImageIndex;
    int _currentFrameIndex{0};
    bool _isFrameStarted{false};
};

} // namespace Core::Rendering
