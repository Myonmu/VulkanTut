//
// Created by miska on 2024/08/14.
//

#pragma once


#include <VmaInstance.h>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "ResourceDescriptor.h"
#include "vk_mem_alloc.h"

struct DeviceContext;
struct VulkanAppContext;
class CommandPool;

struct BufferInfo : public VmaAllocatedResourceInfo<BufferInfo>, public ResourceDescriptor{
    VkDeviceSize size;
    VkBufferUsageFlags usageFlags;
    VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    BufferInfo() = default;

    BufferInfo(VkDeviceSize size, VkBufferUsageFlags usageFlags): size(size), usageFlags(usageFlags) {}
    operator VkBufferCreateInfo() const {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usageFlags;
        bufferInfo.sharingMode = sharingMode;
        return bufferInfo;
    }

    bool operator==(const BufferInfo& other) const {
        return size == other.size && usageFlags == other.usageFlags;
    }

    bool operator!=(const BufferInfo& other) const {
        return !(*this == other);
    }

    static BufferInfo createStagingBufferInfo(VkDeviceSize size, bool isPersistent);

    ResourceArchType get_archType() override {
        return ResourceArchType::BUFFER;
    }
};

class Buffer: public VulkanResource<VkBuffer, DeviceContext> {
public:
    Buffer(DeviceContext& context, BufferInfo& info);
    Buffer(DeviceContext& context, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memUsage,
        VmaAllocationCreateFlags flags = VMA_ALLOCATION_CREATE_MAPPED_BIT);
    ~Buffer() override;
    void copyToBufferMemory(const void* sourceData, size_t offset, size_t size) const;
    static void copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer, DeviceContext& ctx, VkDeviceSize size);
    [[nodiscard]] size_t getSize() const {
        return info.size;
    }
protected:
    void create();
    BufferInfo info;
    VmaAllocation vmaAllocation{};
    VmaAllocationInfo vmaAllocationInfo{};
    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
};


