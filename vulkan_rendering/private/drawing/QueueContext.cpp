//
// Created by miska on 2024/10/19.
//

#include "QueueContext.h"
#include "DeviceContext.h"
QueueContext::QueueContext(DeviceContext& ctx, const uint32_t queueFamilyIndex, uint32_t queueIndex):
SubContext(ctx),
queueFamilyIndex(queueFamilyIndex),
queueIndex(queueIndex)
{
    queue = std::make_unique<VulkanQueue>(ctx, queueFamilyIndex, queueIndex);
    commandPool = std::make_unique<CommandPool>(ctx, queueFamilyIndex);
}

QueueContext::~QueueContext() {
    commandPool.reset();
}

LogicalDevice &QueueContext::getLogicalDevice() const {
    return context.get_logicalDevice();
}
