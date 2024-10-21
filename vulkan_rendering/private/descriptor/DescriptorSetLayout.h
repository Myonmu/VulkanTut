//
// Created by miska on 2024/08/21.
//

#pragma once

#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "DescriptorSetLayoutBinding.h"


struct DescriptorContext;

class DescriptorSetLayout:public VulkanResource<VkDescriptorSetLayout, DescriptorContext> {
public:
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
    explicit DescriptorSetLayout(DescriptorContext& ctx, const std::vector<DescriptorSetLayoutBinding> &bindings);
    ~DescriptorSetLayout() override;
};

