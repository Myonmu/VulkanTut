//
// Created by miska on 2024/08/14.
//

#ifndef BUFFER_H
#define BUFFER_H

#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

class CommandPool;

class Buffer: public VulkanResource<VkBuffer> {
public:
    explicit Buffer(VulkanAppContext& context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props);
    ~Buffer() override;
    void bindBufferMemory(const void* sourceData) const;
    static void copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer, VulkanAppContext& ctx, VkDeviceSize size);
protected:
    VkDeviceSize size;
    VkDeviceMemory bufferMemory{};
    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
};



#endif //BUFFER_H
