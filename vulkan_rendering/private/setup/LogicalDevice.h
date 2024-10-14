//
// Created by miska on 2024/02/08.
// https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Logical_device_and_queues
//

#ifndef VULKANTUT_LOGICALDEVICE_H
#define VULKANTUT_LOGICALDEVICE_H


#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

struct VulkanAppContext;

class LogicalDevice final : public VulkanResource<VkDevice, VulkanAppContext> {
public:
    explicit LogicalDevice(VulkanAppContext& context);
    ~LogicalDevice() override;
    VkQueue graphicsQueue{};
    VkQueue presentQueue{};
private:
    void createLogicalDevice(VulkanAppContext& context);
};


#endif //VULKANTUT_LOGICALDEVICE_H
