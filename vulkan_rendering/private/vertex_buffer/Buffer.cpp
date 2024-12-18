//
// Created by miska on 2024/08/14.
//

#include "Buffer.h"

#include <VulkanAppContext.h>

#include "CBC_Misc.h"
#include "DeviceContext.h"
#include "FrameInfo.h"


BufferInfo BufferInfo::createStagingBufferInfo(VkDeviceSize size, bool isPersistent) {
    BufferInfo info{size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT};
    info.isStagingBuffer(isPersistent);
    return info;
}


void Buffer::create() {
    VkBufferCreateInfo bufferInfo = info;
    VmaAllocationCreateInfo allocationInfo = info; // yeah, funny multiple implicit conversions
    if (vmaCreateBuffer(ctx.get_vma(), &bufferInfo, &allocationInfo,
                        &resource, &vmaAllocation, &vmaAllocationInfo) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer");
    }
}

Buffer::Buffer(DeviceContext &context, BufferInfo &info): VulkanResource(context), info(info) {
    create();
}

Buffer::Buffer(DeviceContext &context, VkDeviceSize size, VkBufferUsageFlags usage,
               VmaMemoryUsage memUsage, VmaAllocationCreateFlags flags)
    : VulkanResource(context), info( size, usage ) {
    info.memoryUsage = memUsage;
    info.allocationFlags = flags;
    create();
    /* Replaced by VMA
    if (vkCreateBuffer(context.getLogicalDevice(), &bufferInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer");
    }

    //Mem allocation
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(context.getLogicalDevice(), resource, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, props);
    if (vkAllocateMemory(context.getLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory");
    }
    //Bind mem
    vkBindBufferMemory(context.getLogicalDevice(), resource, bufferMemory, 0);
    */
}

Buffer::~Buffer() {
    vmaDestroyBuffer(ctx.get_vma(), resource, vmaAllocation);
    //vkDestroyBuffer(ctx.getLogicalDevice(), resource, nullptr);
    //vkFreeMemory(ctx.getLogicalDevice(), bufferMemory, nullptr);
}

uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(ctx.get_physicalDevice(), &memProperties);
    for (auto i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & 1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type");
}


// Uploads source data to the buffer (there will be memcpy)
void Buffer::copyToBufferMemory(const void *sourceData, size_t offset, size_t size) const {
    if (info.allocationFlags & VMA_ALLOCATION_CREATE_MAPPED_BIT) {
        memcpy(static_cast<char *>(vmaAllocationInfo.pMappedData) + offset, sourceData, size);
    } else {
        void *data;
        vmaMapMemory(ctx.get_vma(), vmaAllocation, &data);
        memcpy(static_cast<char *>(data) + offset, sourceData, size);
        vmaUnmapMemory(ctx.get_vma(), vmaAllocation);
    }
}

void Buffer::copyBuffer(Buffer &srcBuffer, Buffer &dstBuffer, DeviceContext &ctx, VkDeviceSize size) {
    CommandBuffer cmd{ctx, QueueFamily::QUEUE_FAMILY_GRAPHICS};
    CommandBufferRecorder recorder{};

    recorder.enqueueCommand<CBC_Misc>(srcBuffer, dstBuffer, 0, 0, size);
    recorder.recordCommandBuffer(cmd, ctx, FrameInfo::DONT_CARE);

    cmd.executeImmediate();
}
