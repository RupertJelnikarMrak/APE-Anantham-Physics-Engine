#include "Core/Ecs/Systems/PointLightRenderSystem.hpp"
#include "Core/Ecs/Components/Components.hpp"
#include "Core/Rendering/GraphicsDevice.hpp"

#include <entt/entt.hpp>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <spdlog/spdlog.h>

// std
#include <cassert>
#include <map>
#include <stdexcept>

namespace Core::Ecs::Systems
{

struct PointLightPushConstants {
    glm::vec4 transform{}; // w is radius
    glm::vec4 color{};     // w is intensity
};

PointLightRenderSystem::PointLightRenderSystem(
    Rendering::GraphicsDevice &device,
    VkRenderPass renderPass,
    VkDescriptorSetLayout globalSetLayout)
    : _device{device}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

PointLightRenderSystem::~PointLightRenderSystem()
{
    vkDestroyPipelineLayout(_device.getDevice(), _pipelineLayout, nullptr);
}

void PointLightRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PointLightPushConstants);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(_device.getDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void PointLightRenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    Rendering::PipelineConfigInfo pipelineConfig{};
    Rendering::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    Rendering::Pipeline::enableAlphaBlending(pipelineConfig);
    pipelineConfig.attributeDescriptions.clear();
    pipelineConfig.bindingDescriptions.clear();
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = _pipelineLayout;
    _pipeline = std::make_unique<Rendering::Pipeline>(
        _device,
        "shaders/point_light.vert.spv",
        "shaders/point_light.frag.spv",
        pipelineConfig);
}

void PointLightRenderSystem::update(Rendering::FrameInfo &frameInfo, Rendering::GlobalUbo &ubo)
{
    using Components::PointLight;

    // Sort the lights by distance to camer
    std::map<float, entt::entity> sorted;
    frameInfo.registry.view<const PointLight>().each([&](const entt::entity entity, const PointLight &pointLight) {
        glm::vec3 offset = frameInfo.camera.getPosition() - pointLight.position;
        float disSquared = glm::dot(offset, offset);
        sorted[disSquared] = entity;
    });
}

void PointLightRenderSystem::render(Rendering::FrameInfo &frameInfo, Rendering::GlobalUbo &ubo)
{
    using Components::PointLight;

    // Sort the lights by distance to camer
    std::map<float, entt::entity> sorted;
    frameInfo.registry.view<const PointLight>().each([&](const entt::entity entity, const PointLight &pointLight) {
        glm::vec3 offset = frameInfo.camera.getPosition() - pointLight.position;
        float disSquared = glm::dot(offset, offset);
        sorted[disSquared] = entity;
    });

    _pipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(
        frameInfo.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        _pipelineLayout,
        0,
        1,
        &frameInfo.globalDescriptorSet,
        0,
        nullptr);

    ubo.numLights = 0;

    for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
        auto entity = it->second;
        const PointLight &light = frameInfo.registry.get<const PointLight>(entity);

        PointLightPushConstants push{};
        push.transform = glm::vec4(light.position, light.radius);
        push.color = glm::vec4(light.color, light.intensity);

        vkCmdPushConstants(
            frameInfo.commandBuffer,
            _pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(PointLightPushConstants),
            &push);
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);

        // For now lights can only statically be passed to the shader in the form of a uniform buffer
        // That means that there is a static max allowed number of lights for the shader
        // The global ubo holds this information together with an array of lights which are passed to the shader
        ubo.pointLights[ubo.numLights].position = glm::vec4(light.position, 1.f);
        ubo.pointLights[ubo.numLights].color = glm::vec4(light.color, light.intensity);
        ubo.numLights++;

        if (ubo.numLights > MAX_LIGHTS) {
            SPDLOG_INFO(
                "Point lights exceed maximum specified, the ones farthest from the camera will not be rendered");
            break;
        }
    }
}

} // namespace Core::Ecs::Systems
