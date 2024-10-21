//
// Created by miska on 2024/02/08.
// https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Logical_device_and_queues
//

#pragma once


#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

struct DeviceContext;

class LogicalDevice final : public VulkanResource<VkDevice, DeviceContext> {
public:
    explicit LogicalDevice(DeviceContext& context);
    ~LogicalDevice() override;
private:
    void createLogicalDevice(DeviceContext& context);
};


