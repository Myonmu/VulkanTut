//
// Created by miska on 2024/08/21.
//

#include "DescriptorSetLayout.h"

#include <ranges>
#include <VulkanAppContext.h>
#include "DeviceContext.h"

DescriptorSetLayout::DescriptorSetLayout(DeviceContext &ctx, uint32_t setId, const std::vector<DescriptorSetLayoutBinding> &bindings)
    : VulkanResource(ctx), setId(setId) {
    layoutBindings.resize(bindings.size());
    for (int i = 0; i < layoutBindings.size(); ++i) {
        layoutBindings[i] = bindings[i].asVkDescriptorSetLayoutBinding();
        if (auto type = layoutBindings[i].descriptorType; requirements.contains(type)) {
            requirements[type]++;
        }else {
            requirements[type] = 1;
        }
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = layoutBindings.data();

    if (vkCreateDescriptorSetLayout(ctx.getLogicalDevice(), &layoutInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::DescriptorSetLayout(DeviceContext& ctx, uint32_t setId, const std::map<uint32_t, DescriptorSetLayoutBinding> &bindings)
: VulkanResource(ctx) , setId(setId){
    for (const auto &snd: bindings | std::views::values) {
        layoutBindings.push_back(snd.asVkDescriptorSetLayoutBinding());
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

DescriptorAllocPolicy DescriptorSetLayout::getAllocPolicy() const {
    if (setId == 0) return DescriptorAllocPolicy::ENGINE_DEFINED;
    for (auto binding: layoutBindings) {
        if (binding.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT) {
            return DescriptorAllocPolicy::PER_SWAPCHAIN;
        }
    }
    return DescriptorAllocPolicy::PER_MATERIAL_INSTANCE;
}
