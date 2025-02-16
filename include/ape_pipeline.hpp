#pragma once

#include "ape_device.hpp"
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ape
{

struct PipelineConfigInfo {
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo &) = delete;
    PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class ApePipeline
{
public:
    ApePipeline(
        ApeDevice &device,
        const std::string &vertFilepath,
        const std::string &fragFilepath,
        const PipelineConfigInfo &configInfo);
    ~ApePipeline();

    ApePipeline(const ApePipeline &) = delete;
    ApePipeline &operator=(const ApePipeline &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

private:
    static std::vector<char> readFile(const std::string &filepath);

    void createGraphicsPipeline(
        const std::string &vertFilepath,
        const std::string &fragFilepath,
        const PipelineConfigInfo &configInfo);

    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    ApeDevice &apeDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
} // namespace ape
