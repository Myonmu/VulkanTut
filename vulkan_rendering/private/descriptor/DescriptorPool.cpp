//
// Created by miska on 2024/08/23.
//

#include "DescriptorPool.h"

#include <VulkanAppContext.h>

#include "DeviceContext.h"

DescriptorPool::DescriptorPool(DeviceContext &ctx, uint32_t maxSets, std::vector<VkDescriptorPoolSize>& sizes)
: VulkanResource(ctx) {
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
    poolInfo.pPoolSizes = sizes.data();
    poolInfo.maxSets = maxSets;

    if(vkCreateDescriptorPool(ctx.getLogicalDevice(), &poolInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool() {
    vkDestroyDescriptorPool(ctx.getLogicalDevice(), resource, nullptr);
}

void DescriptorPool::reset(const VkDescriptorPoolResetFlags flags) const {
    vkResetDescriptorPool(ctx.getLogicalDevice(), resource, flags);
}


