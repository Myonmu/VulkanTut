//
// Created by miska on 2024/02/11.
//

#pragma once

#include "VulkanResource.h"
#include "ShaderModule.h"
#include <vulkan/vulkan_core.h>
class SwapChain;
class RenderPass;
class PipelineLayout;

// Per-object rendering pipeline
class VulkanPipeline : public VulkanResource<VkPipeline, DeviceContext> {
public:
    explicit VulkanPipeline(
        DeviceContext &context,
        const std::vector<Shader> &shaders,
        const PipelineLayout &layout,
        const RenderPass &renderPass,
        uint32_t subpassId
    );

    ~VulkanPipeline() override;


    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkPipelineDynamicStateCreateInfo dynamicState{};
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    VkPipelineViewportStateCreateInfo viewportState{};
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    VkGraphicsPipelineCreateInfo pipelineInfo{};

private:
    const RenderPass& renderPass;
    uint32_t subpassId;
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    std::vector<std::unique_ptr<ShaderModule> > shaderModules;
};
