//
// Created by miska on 2024/08/23.
//

#pragma once
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct DeviceContext;

/*
A descriptor pool maintains a pool of descriptors, from which descriptor sets are allocated.
Descriptor pools are externally synchronized, meaning that the application must not allocate
and/or free descriptor sets from the same pool in multiple threads simultaneously.
*/
class DescriptorPool: public VulkanResource<VkDescriptorPool, DeviceContext>{
public:
    explicit DescriptorPool(DeviceContext& ctx, uint32_t maxSets, std::vector<VkDescriptorPoolSize>& sizes);
    ~DescriptorPool() override;
    void reset(VkDescriptorPoolResetFlags flags) const;
};
