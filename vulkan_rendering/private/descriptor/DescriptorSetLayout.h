//
// Created by miska on 2024/08/21.
//

#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "DescriptorSetLayoutBinding.h"


struct DeviceContext;

class DescriptorSetLayout:public VulkanResource<VkDescriptorSetLayout, DeviceContext> {

public:
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
    std::unordered_map<VkDescriptorType, uint32_t> requirements{};
    DescriptorSetLayout(DeviceContext& ctx, const std::vector<DescriptorSetLayoutBinding> &bindings);
    DescriptorSetLayout(DeviceContext& ctx, const std::map<uint32_t, DescriptorSetLayoutBinding> &bindings);
    ~DescriptorSetLayout() override;
};

