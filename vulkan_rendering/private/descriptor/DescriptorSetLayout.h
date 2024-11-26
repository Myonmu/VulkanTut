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
#include "DescriptorSetLayout.h"


enum class DescriptorAllocPolicy {
    ENGINE_DEFINED,
    PER_SWAPCHAIN,
    PER_FRAME,
    PER_MATERIAL,
    PER_MATERIAL_INSTANCE,
};

struct DeviceContext;

class DescriptorSetLayout:public VulkanResource<VkDescriptorSetLayout, DeviceContext> {

public:
    uint32_t setId;
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
    // Type -> count
    std::unordered_map<VkDescriptorType, uint32_t> requirements{};
    DescriptorSetLayout(DeviceContext& ctx, uint32_t setId, const std::vector<DescriptorSetLayoutBinding> &bindings);
    DescriptorSetLayout(DeviceContext& ctx, uint32_t setId, const std::map<uint32_t, DescriptorSetLayoutBinding> &bindings);
    ~DescriptorSetLayout() override;
    [[nodiscard]] DescriptorAllocPolicy getAllocPolicy() const;
};

