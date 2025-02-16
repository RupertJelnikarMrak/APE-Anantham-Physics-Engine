#include "first_app.hpp"
#include "ape_pipeline.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <cassert>
#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ape
{

struct SimplePushConstantData {
    alignas(8) glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

FirstApp::FirstApp()
{
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

FirstApp::~FirstApp() { vkDestroyPipelineLayout(apeDevice.device(), pipelineLayout, nullptr); }

void FirstApp::run()
{
    while (!apeWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(apeDevice.device());
}

void FirstApp::loadModels()
{
    std::vector<ApeModel::Vertex> vertices{
        {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}},
        {{0.0f, -0.5f}, {0.0f, 1.0f, 1.0f}},
    };
    apeModel = std::make_unique<ApeModel>(apeDevice, vertices);
}

void FirstApp::createPipelineLayout()
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(apeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void FirstApp::recreateSwapChain()
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
        apeSwapChain = std::make_unique<ApeSwapChain>(apeDevice, extent, std::move(apeSwapChain));
        if (apeSwapChain->imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }

    createPipeline();
}

void FirstApp::createPipeline()
{
    assert(apeSwapChain != nullptr && "Cannot create pipeline before swap chain");
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    ApePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = apeSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    apePipeline = std::make_unique<ApePipeline>(
        apeDevice,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
}

void FirstApp::createCommandBuffers()
{
    commandBuffers.resize(apeSwapChain->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = apeDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(apeDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void FirstApp::freeCommandBuffers()
{
    vkFreeCommandBuffers(
        apeDevice.device(),
        apeDevice.getCommandPool(),
        static_cast<uint32_t>(commandBuffers.size()),
        commandBuffers.data());
    commandBuffers.clear();
}

void FirstApp::recordCommandBuffer(int imageIndex)
{
    static int frame = 0;
    frame = (frame + 1) % 10000;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = apeSwapChain->getRenderPass();
    renderPassInfo.framebuffer = apeSwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = apeSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(apeSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(apeSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, apeSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    apePipeline->bind(commandBuffers[imageIndex]);
    apeModel->bind(commandBuffers[imageIndex]);

    for (int j = 0; j < 3; j++) {
        SimplePushConstantData push{};
        push.offset = {-0.5f + frame * 0.0002f, -0.4f + j * 0.25f};
        push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

        vkCmdPushConstants(
            commandBuffers[imageIndex],
            pipelineLayout,
            VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof(SimplePushConstantData),
            &push);

        apeModel->draw(commandBuffers[imageIndex]);
    }

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void FirstApp::drawFrame()
{
    uint32_t imageIndex;
    auto result = apeSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire next swap chain image!");
    }

    recordCommandBuffer(imageIndex);
    result = apeSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || apeWindow.wasWindowResized()) {
        apeWindow.resertWindowResizedFlag();
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to submit command buffer!");
    }
}

} // namespace ape
