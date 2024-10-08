//
// Created by miska on 2024/08/23.
//

#include "DescriptorPool.h"

#include <VulkanAppContext.h>

DescriptorPool::DescriptorPool(VulkanAppContext &ctx): VulkanResource<VkDescriptorPool_T*>(ctx) {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(ctx.MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(ctx.MAX_FRAMES_IN_FLIGHT);

    if(vkCreateDescriptorPool(ctx.logicalDevice, &poolInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool() {
    vkDestroyDescriptorPool(ctx.logicalDevice, resource, nullptr);
}

