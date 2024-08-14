//
// Created by miska on 2024/08/14.
//

#include "Buffer.h"

#include <VulkanAppContext.h>


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


void Buffer::bindBufferMemory(const void* sourceData) const {
    void *data;
    vkMapMemory(ctx.logicalDevice, bufferMemory, 0, size, 0, &data);
    memcpy(data, sourceData, size);
    vkUnmapMemory(ctx.logicalDevice, bufferMemory);
}

void Buffer::copyBuffer(Buffer &srcBuffer, Buffer &dstBuffer, VulkanAppContext& ctx, VkDeviceSize size) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = ctx.commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(ctx.logicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(ctx.logicalDevice.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(ctx.logicalDevice.graphicsQueue);
    vkFreeCommandBuffers(ctx.logicalDevice, ctx.commandPool, 1, &commandBuffer);

}
