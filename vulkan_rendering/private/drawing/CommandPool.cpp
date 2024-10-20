//
// Created by miska on 2024/08/04.
//

#include "CommandPool.h"

#include <QueueFamilyIndices.h>
#include <VulkanAppContext.h>

#include "DeviceContext.h"

CommandPool::CommandPool(DeviceContext &context, uint32_t queueFamilyIndex, VkCommandPoolCreateFlagBits flags) : VulkanResource(context){
    VkCommandPoolCreateInfo poolInfo{};

    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    /*
    There are two possible flags for command pools:
    VK_COMMAND_POOL_CREATE_TRANSIENT_BIT:
    Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
    VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT:
    Allow command buffers to be rerecorded individually, without this flag they all have to be reset together

    We will be recording a command buffer every frame,
    so we want to be able to reset and rerecord over it.
    Thus, we need to set the VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT flag bit for our command pool.
     */
    poolInfo.flags = flags;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    /*
    Command buffers are executed by submitting them on one of the device queues,
    like the graphics and presentation queues we retrieved.
    Each command pool can only allocate command buffers that are submitted on a single type of queue.
    We're going to record commands for drawing, which is why we've chosen the graphics queue family.
     */
    if (vkCreateCommandPool(context.getLogicalDevice(), &poolInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool(ctx.getLogicalDevice(), resource, nullptr);
}
