//
// Created by miska on 2024/10/14.
//

#include "DeviceContext.h"

#include <VulkanAppContext.h>

#include "QueueFamilyIndices.h"

DeviceContext::DeviceContext(const VulkanAppContext &ctx) : SubContext(ctx) {
}

void DeviceContext::createWindow(const char *name, int width, int height, QueueFamily requiredQueueFamilies) {
    // when the logical device is not yet created, adding a new window (surface) is safe
    if (!isDeviceCreated) {
        windowContexts.emplace_back(*this, name, width, height, requiredQueueFamilies);
    } else {
        // or when required queue families are already satisfied
        if (auto combinedRequirements = getCombinedQueueFamilyRequirements();
            (combinedRequirements | requiredQueueFamilies) == combinedRequirements) {

            windowContexts.emplace_back(*this, name, width, height, requiredQueueFamilies);

        } else {
            // or else, requiring a new queue family would cause a device recreation
            // we do not consider allocating a new physical device for now.
            // TODO: allow allocating new physical device for unsuitable surface
            // TODO: allow device recreation
            throw std::runtime_error(
                "New window requires extra queue families which will cause device recreation, this is not supported.");
        }
    }
}


void DeviceContext::init() {

    physicalDevice = std::make_unique<PhysicalDevice>(*this);
    logicalDevice = std::make_unique<LogicalDevice>(*this);
    commandPool = std::make_unique<CommandPool>(*this);

    isDeviceCreated = true;
    // now we finalize window contexts
    for (auto& window: windowContexts) {
        window.init();
    }
}

DeviceContext::~DeviceContext() = default;

const LogicalDevice &DeviceContext::getLogicalDevice() const {
    return get_logicalDevice();
}

QueueFamily DeviceContext::getCombinedQueueFamilyRequirements() const {
    QueueFamily q{};
    for (const auto &window: windowContexts) {
        q = q | window.requiredQueueFamilies;
    }
    return q;
}

CTX_FORWARD_GET_BODY(DeviceContext, VulkanInstance, vulkanInstance)
