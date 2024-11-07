//
// Created by miska on 2024/08/27.
//

#include "DescriptorSetLayout.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "DeviceContext.h"

DescriptorSet::DescriptorSet(DeviceContext &ctx, const DescriptorPool& pool, const DescriptorSetLayout& layout)
: VulkanResource(ctx), layout(layout) {
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = layout;
    const auto result = vkAllocateDescriptorSets(ctx.getLogicalDevice(), &allocInfo, &resource);
    if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL) {
        throw DescriptorPoolOutOfMemoryException("Descriptor Pool does not have enough memory");
    }
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
}

DescriptorSet::~DescriptorSet() = default;