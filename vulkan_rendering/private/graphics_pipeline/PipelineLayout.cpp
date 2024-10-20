//
// Created by miska on 2024/02/11.
//

#include "PipelineLayout.h"

#include "DeviceContext.h"
#include "VulkanAppContext.h"

PipelineLayout::PipelineLayout(DeviceContext &context, std::vector<VkDescriptorSetLayout> layouts): VulkanResource(context) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = layouts.size(); // Optional
    pipelineLayoutInfo.pSetLayouts = layouts.data(); // Optional
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
