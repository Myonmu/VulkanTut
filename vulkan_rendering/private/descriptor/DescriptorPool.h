//
// Created by miska on 2024/08/23.
//

#pragma once
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct DescriptorContext;

/*
A descriptor pool maintains a pool of descriptors, from which descriptor sets are allocated.
Descriptor pools are externally synchronized, meaning that the application must not allocate
and/or free descriptor sets from the same pool in multiple threads simultaneously.
*/
class DescriptorPool: public VulkanResource<VkDescriptorPool, DescriptorContext>{
public:
    explicit DescriptorPool(DescriptorContext& ctx);
    ~DescriptorPool() override;
};
