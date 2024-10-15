//
// Created by miska on 2024/10/14.
//

#include "DeviceContext.h"
#include "QueueFamilyIndices.h"

DeviceContext::DeviceContext(const VulkanAppContext &ctx) : SubContext(ctx){
}


DeviceContext::~DeviceContext() = default;

const LogicalDevice &DeviceContext::getLogicalDevice() const {
    return get_logicalDevice();
}

QueueFamily DeviceContext::getCombinedQueueFamilyRequirements() const {
    QueueFamily q{};
    for (const auto& window: windowContexts) {
        q = q | window.requiredQueueFamilies;
    }
    return q;
}

CTX_FORWARD_GET_BODY(DeviceContext, VulkanInstance, vulkanInstance)

