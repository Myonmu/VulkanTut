//
// Created by miska on 2024/08/04.
//

#include "CommandBuffer.h"

#include <VulkanAppContext.h>

#include "DeviceContext.h"

CommandBuffer::CommandBuffer(DeviceContext &context): VulkanResource(context){
    VkCommandBufferAllocateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = context.get_commandPool();
    /*
    The level parameter specifies if the allocated command buffers are primary or secondary command buffers.

    VK_COMMAND_BUFFER_LEVEL_PRIMARY:
    Can be submitted to a queue for execution, but cannot be called from other command buffers.

    VK_COMMAND_BUFFER_LEVEL_SECONDARY:
    Cannot be submitted directly, but can be called from primary command buffers.

    We won't make use of the secondary command buffer functionality here,
    but you can imagine that it's helpful to reuse common operations from primary command buffers.
    Since we are only allocating one command buffer, the commandBufferCount parameter is just one.
     */
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(context.getLogicalDevice(), &info, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

CommandBuffer::~CommandBuffer() {
    vkFreeCommandBuffers(ctx.getLogicalDevice(), ctx.get_commandPool(), 1, *this);
};

void CommandBuffer::executeImmediate() {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = *this;
    vkQueueSubmit(ctx.getLogicalDevice().graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(ctx.getLogicalDevice().graphicsQueue);
}