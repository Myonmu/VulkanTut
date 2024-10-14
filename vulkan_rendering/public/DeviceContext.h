//
// Created by miska on 2024/10/14.
//

#ifndef DEVICECONTEXT_H
#define DEVICECONTEXT_H
#include <LogicalDevice.h>
#include <PhysicalDevice.h>
#include <vector>

#include "SubContext.h"


struct VulkanAppContext;

struct DeviceContext : SubContext<VulkanAppContext> {
    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
     PhysicalDevice physicalDevice{*this};
     LogicalDevice logicalDevice{*this};
    explicit DeviceContext(const VulkanAppContext& ctx);
    ~DeviceContext() override;
    const LogicalDevice &getLogicalDevice() const override;
};


#endif //DEVICECONTEXT_H
