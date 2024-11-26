//
// Created by miska on 2024/08/27.
//

#pragma once
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>
#include "DeviceContext.h"

class DescriptorPool;
class DescriptorSetLayout;

class DescriptorPoolOutOfMemoryException final : public std::runtime_error {
public:
    explicit DescriptorPoolOutOfMemoryException(const std::string& what) : std::runtime_error(what) {}
};

class DescriptorSet : public VulkanResource<VkDescriptorSet, DeviceContext> {
public:
    const DescriptorSetLayout& layout;
    explicit DescriptorSet(DeviceContext& ctx, const DescriptorPool& pool, const DescriptorSetLayout& layout);
    ~DescriptorSet() override;
};
