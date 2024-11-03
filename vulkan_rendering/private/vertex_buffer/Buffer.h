//
// Created by miska on 2024/08/14.
//

#pragma once


#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "vk_mem_alloc.h"

struct DeviceContext;
struct VulkanAppContext;
class CommandPool;

class Buffer: public VulkanResource<VkBuffer, DeviceContext> {
public:
    explicit Buffer(DeviceContext& context, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memUsage,
        VmaAllocationCreateFlags flags = VMA_ALLOCATION_CREATE_MAPPED_BIT);
    ~Buffer() override;
    void copyToBufferMemory(const void* sourceData, size_t offset) const;
    static void copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer, DeviceContext& ctx, VkDeviceSize size);
    // this is a reference to a pointer, because mapping buffer memory would set the pointer as well.
    void mapBufferMemory(void*& source) const;
protected:
    VkDeviceSize size{};
    VkDeviceMemory bufferMemory{};
    VmaAllocation vmaAllocation{};
    VmaAllocationInfo vmaAllocationInfo{};
    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
};


