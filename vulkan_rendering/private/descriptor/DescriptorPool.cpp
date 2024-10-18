//
// Created by miska on 2024/08/23.
//

#include "DescriptorPool.h"

#include <VulkanAppContext.h>

#include "DescriptorContext.h"
#include "DeviceContext.h"

DescriptorPool::DescriptorPool(DescriptorContext &ctx): VulkanResource(ctx) {
    auto maxFramesInFlight = ctx.context.context.MAX_FRAMES_IN_FLIGHT;
    std::vector<VkDescriptorPoolSize> poolSizes{ctx.get_descriptorSetLayout().layoutBindings.size()};

    for (int i = 0; i < poolSizes.size(); i++) {
        poolSizes[i].type = ctx.get_descriptorSetLayout().layoutBindings[i].descriptorType;
        poolSizes[i].descriptorCount = static_cast<uint32_t>(maxFramesInFlight);
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(maxFramesInFlight);

    if(vkCreateDescriptorPool(ctx.getLogicalDevice(), &poolInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool() {
    vkDestroyDescriptorPool(ctx.getLogicalDevice(), resource, nullptr);
}

