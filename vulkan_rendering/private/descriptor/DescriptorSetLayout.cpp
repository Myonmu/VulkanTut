//
// Created by miska on 2024/08/21.
//

#include "DescriptorSetLayout.h"
#include <VulkanAppContext.h>
#include "DescriptorContext.h"
#include "LogicalDevice.h"

DescriptorSetLayout::DescriptorSetLayout(DescriptorContext &ctx, const std::vector<DescriptorSetLayoutBinding> &bindings)
    : VulkanResource(ctx) {
    layoutBindings.resize(bindings.size());
    for (int i = 0; i < layoutBindings.size(); ++i) {
        VkDescriptorSetLayoutBinding b{};
        b.binding = i;
        b.descriptorCount = 1;
        b.descriptorType = bindings[i].type;
        b.stageFlags = bindings[i].stageFlags;
        b.pImmutableSamplers = nullptr;
        layoutBindings[i] = b;
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = layoutBindings.data();

    if (vkCreateDescriptorSetLayout(ctx.getLogicalDevice(), &layoutInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::~DescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(ctx.getLogicalDevice(), resource, nullptr);
}
