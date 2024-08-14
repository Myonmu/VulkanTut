//
// Created by miska on 2024/02/11.
//

#include "GraphicsPipeline.h"
#include "VulkanAppContext.h"

GraphicsPipeline::GraphicsPipeline(VulkanAppContext &context) {
    ctx = &context;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(context.logicalDevice->getRaw(),
                               &pipelineLayoutInfo,
                               nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

GraphicsPipeline::~GraphicsPipeline() {
    vkDestroyPipelineLayout(ctx->logicalDevice->getRaw(), resource, nullptr);
}
