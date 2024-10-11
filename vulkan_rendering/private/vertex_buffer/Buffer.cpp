//
// Created by miska on 2024/08/14.
//

#include "Buffer.h"

#include <VulkanAppContext.h>

#include "CopyBuffer.h"
#include "FrameInfo.h"


Buffer::Buffer(VulkanAppContext &context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props) :
VulkanResource<VkBuffer_T*>(context){
    this->size = size;
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if(vkCreateBuffer(context.logicalDevice,&bufferInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer");
    }
    //Mem allocation
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(context.logicalDevice, resource, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, props);
    if(vkAllocateMemory(context.logicalDevice,&allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory");
    }
    //Bind mem
    vkBindBufferMemory(context.logicalDevice, resource, bufferMemory, 0);
}

Buffer::~Buffer() {
    vkDestroyBuffer(ctx.logicalDevice, resource, nullptr);
    vkFreeMemory(ctx.logicalDevice, bufferMemory, nullptr);
}

uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(ctx.physicalDevice, &memProperties);
    for(auto i = 0; i < memProperties.memoryTypeCount; i++) {
        if( (typeFilter & 1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties ) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type");
}


// Uploads source data to the buffer (there will be memcpy)
void Buffer::bindBufferMemory(const void* sourceData) const {
    void *data;
    vkMapMemory(ctx.logicalDevice, bufferMemory, 0, size, 0, &data);
    memcpy(data, sourceData, size);
    vkUnmapMemory(ctx.logicalDevice, bufferMemory);
}

// Projects buffer memory (the source param will be filled with buffer memory position)
void Buffer::mapBufferMemory(void*& source) const {
    vkMapMemory(ctx.logicalDevice, bufferMemory, 0, size, 0, &source);
}

void Buffer::copyBuffer(Buffer &srcBuffer, Buffer &dstBuffer, VulkanAppContext& ctx, VkDeviceSize size) {
    CommandBuffer cmd{ctx};
    CommandBufferRecorder recorder{};

    recorder.enqueueCommand<CopyBuffer>(srcBuffer, dstBuffer, 0, 0, size);
    recorder.recordCommandBuffer(cmd, ctx, FrameInfo::DONT_CARE);

    cmd.executeImmediate();
}
