//
// Created by miska on 2024/02/08.
// https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Logical_device_and_queues
//

#ifndef VULKANTUT_LOGICALDEVICE_H
#define VULKANTUT_LOGICALDEVICE_H


#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"

struct VulkanAppContext;

class LogicalDevice: public VulkanResource<VkDevice> {
public:
    explicit LogicalDevice(VulkanAppContext& context);
    ~LogicalDevice() override;
    VkQueue graphicsQueue{};
    VkQueue presentQueue{};
private:
    void createLogicalDevice(VulkanAppContext& context);
};


#endif //VULKANTUT_LOGICALDEVICE_H
