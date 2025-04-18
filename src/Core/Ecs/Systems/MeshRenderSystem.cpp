#include "Core/Ecs/Systems/MeshRenderSystem.hpp"
#include "Core/Ecs/Components/Components.hpp"
#include "Core/Resources/ResourceManager.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <cassert>

namespace Core::Ecs::Systems
{

struct SimplePushConstantData {
    glm::mat4 modelMatrix{1.f};
    glm::mat4 normalMatrix{1.f};
};

MeshRenderSystem::MeshRenderSystem(
    Rendering::GraphicsDevice &device,
    VkRenderPass renderPass,
    VkDescriptorSetLayout globalSetLayout,
    Resources::ResourceManager &ResourceManager)
    : _device{device}, _resourceManager{ResourceManager}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

MeshRenderSystem::~MeshRenderSystem() { vkDestroyPipelineLayout(_device.getDevice(), _pipelineLayout, nullptr); }

void MeshRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

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

void MeshRenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    Rendering::PipelineConfigInfo pipelineConfig{};
    Rendering::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = _pipelineLayout;
    _pipeline = std::make_unique<Rendering::Pipeline>(
        _device,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
}

glm::mat4 mat4(const Components::Transform &transform)
{
    const float c3 = glm::cos(transform.rotation.z);
    const float s3 = glm::sin(transform.rotation.z);
    const float c2 = glm::cos(transform.rotation.x);
    const float s2 = glm::sin(transform.rotation.x);
    const float c1 = glm::cos(transform.rotation.y);
    const float s1 = glm::sin(transform.rotation.y);
    return glm::mat4{
        {
            transform.scale.x * (c1 * c3 + s1 * s2 * s3),
            transform.scale.x * (c2 * s3),
            transform.scale.x * (c1 * s2 * s3 - c3 * s1),
            0.0f,
        },
        {
            transform.scale.y * (c3 * s1 * s2 - c1 * s3),
            transform.scale.y * (c2 * c3),
            transform.scale.y * (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            transform.scale.z * (c2 * s1),
            transform.scale.z * (-s2),
            transform.scale.z * (c1 * c2),
            0.0f,
        },
        {transform.translation.x, transform.translation.y, transform.translation.z, 1.0f}};
}

glm::mat3 normalMatrix(const Components::Transform &transform)
{
    const float c3 = glm::cos(transform.rotation.z);
    const float s3 = glm::sin(transform.rotation.z);
    const float c2 = glm::cos(transform.rotation.x);
    const float s2 = glm::sin(transform.rotation.x);
    const float c1 = glm::cos(transform.rotation.y);
    const float s1 = glm::sin(transform.rotation.y);
    const glm::vec3 invScale = 1.0f / transform.scale;

    return glm::mat3{
        {
            invScale.x * (c1 * c3 + s1 * s2 * s3),
            invScale.x * (c2 * s3),
            invScale.x * (c1 * s2 * s3 - c3 * s1),
        },
        {
            invScale.y * (c3 * s1 * s2 - c1 * s3),
            invScale.y * (c2 * c3),
            invScale.y * (c1 * c3 * s2 + s1 * s3),
        },
        {
            invScale.z * (c2 * s1),
            invScale.z * (-s2),
            invScale.z * (c1 * c2),
        },
    };
}

void MeshRenderSystem::render(Rendering::FrameInfo &frameInfo)
{
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

    frameInfo.registry.view<const Components::Transform, const Components::Mesh>().each(
        [&](auto entity, const Components::Transform &transform, const Components::Mesh &mesh) {
            SimplePushConstantData push{};
            push.modelMatrix = mat4(transform);
            push.normalMatrix = normalMatrix(transform);

            vkCmdPushConstants(
                frameInfo.commandBuffer,
                _pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);

            auto meshObj = _resourceManager.meshCache.get(mesh.meshName);
            meshObj->bind(frameInfo.commandBuffer);
            meshObj->draw(frameInfo.commandBuffer);
        });
}

} // namespace Core::Ecs::Systems
