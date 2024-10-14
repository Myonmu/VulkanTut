//
// Created by miska on 2024/10/14.
//

#include "DeviceContext.h"

DeviceContext::DeviceContext(const VulkanAppContext &ctx) : SubContext(ctx){
}

DeviceContext::~DeviceContext() = default;

const LogicalDevice &DeviceContext::getLogicalDevice() const {
    return logicalDevice;
}

