//
// Created by miska on 2024/10/19.
//

#include "VulkanQueue.h"

#include "DeviceContext.h"

VulkanQueue::VulkanQueue(DeviceContext& ctx, uint32_t queueFamilyIndex, uint32_t queueIndex):
VulkanResource(ctx)
{
    vkGetDeviceQueue(ctx.getLogicalDevice(), queueFamilyIndex, queueIndex, &resource);
}

VulkanQueue::~VulkanQueue() = default;

