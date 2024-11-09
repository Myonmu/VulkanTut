//
// Created by miska on 2024/02/11.
//

#include "PipelineLayout.h"

#include "DeviceContext.h"
#include "RenderingContext.h"

PipelineLayout::PipelineLayout(DeviceContext &context, const std::vector<VkDescriptorSetLayout> &layouts)
: VulkanResource(context) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    const auto pushConstants = PerObjectVertexPushConstants::getPushConstantsRange();
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = layouts.size(); // Optional
    pipelineLayoutInfo.pSetLayouts = layouts.data(); // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
    pipelineLayoutInfo.pPushConstantRanges = &pushConstants; // Optional

    if (vkCreatePipelineLayout(ctx.getLogicalDevice(),
                               &pipelineLayoutInfo,
                               nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

PipelineLayout::~PipelineLayout() {
    vkDestroyPipelineLayout(ctx.getLogicalDevice(), resource, nullptr);
}
