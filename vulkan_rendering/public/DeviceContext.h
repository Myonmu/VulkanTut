//
// Created by miska on 2024/10/14.
//

#ifndef DEVICECONTEXT_H
#define DEVICECONTEXT_H
#include <CommandPool.h>
#include <LogicalDevice.h>
#include <PhysicalDevice.h>
#include <vector>
#include <VulkanInstance.h>

#include "ContextMacros.h"
#include "SubContext.h"
#include "WindowContext.h"

struct VulkanAppContext;

struct DeviceContext : SubContext<VulkanAppContext> {
    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    std::vector<WindowContext> windowContexts;

    CTX_PROPERTY(QueueFamilyIndices, queueFamilyIndices)
    // requires surface
    CTX_PROPERTY(PhysicalDevice, physicalDevice)
    CTX_PROPERTY(LogicalDevice, logicalDevice)

    CTX_PROPERTY(CommandPool, commandPool)

    CTX_FORWARD_GET_DECL(VulkanInstance, vulkanInstance)

    explicit DeviceContext(const VulkanAppContext &ctx);

    ~DeviceContext() override;

    [[nodiscard]] const LogicalDevice &getLogicalDevice() const override;

    [[nodiscard]] QueueFamily getCombinedQueueFamilyRequirements() const;

    void createWindow(const char *name, int width, int height, QueueFamily requiredQueueFamilies);

    // manually called after all surfaces are created
    void init();

    [[nodiscard]] bool isLogicalDeviceCreated() const {return isDeviceCreated;}


private:
    bool isDeviceCreated = false;
};


#endif //DEVICECONTEXT_H
