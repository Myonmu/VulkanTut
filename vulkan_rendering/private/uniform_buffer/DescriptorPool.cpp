//
// Created by miska on 2024/08/23.
//

#include "DescriptorPool.h"

#include <array>
#include <VulkanAppContext.h>

DescriptorPool::DescriptorPool(VulkanAppContext &ctx): VulkanResource(ctx) {
    std::vector<VkDescriptorPoolSize> poolSizes{ctx.descriptorSetLayout.layoutBindings.size()};

    for (int i = 0; i < poolSizes.size(); i++) {
        poolSizes[i].type = ctx.descriptorSetLayout.layoutBindings[i].descriptorType;
        poolSizes[i].descriptorCount = static_cast<uint32_t>(ctx.MAX_FRAMES_IN_FLIGHT);
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(ctx.MAX_FRAMES_IN_FLIGHT);

    if(vkCreateDescriptorPool(ctx.logicalDevice, &poolInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool() {
    vkDestroyDescriptorPool(ctx.logicalDevice, resource, nullptr);
}

