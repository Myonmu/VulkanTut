//
// Created by miska on 2024/02/11.
//

#include "PipelineLayout.h"

#include "PipelineContext.h"
#include "VulkanAppContext.h"

PipelineLayout::PipelineLayout(PipelineContext &context): VulkanResource(context) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1; // Optional
    pipelineLayoutInfo.pSetLayouts = context.descriptorSetLayout; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(ctx.getLogicalDevice(),
                               &pipelineLayoutInfo,
                               nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

PipelineLayout::~PipelineLayout() {
    vkDestroyPipelineLayout(ctx.getLogicalDevice(), resource, nullptr);
}
