//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_PHYSICALDEVICE_H
#define VULKANTUT_PHYSICALDEVICE_H

#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"

struct VulkanAppContext;

class PhysicalDevice: public VulkanResource<VkPhysicalDevice, VulkanAppContext>{
public:
    explicit PhysicalDevice(VulkanAppContext& context);
private:
    void pickPhysicalDevice(VulkanAppContext& context);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device, VulkanAppContext& context);
    static int rateDeviceSuitability(VkPhysicalDevice device, VulkanAppContext& context);
};


#endif //VULKANTUT_PHYSICALDEVICE_H
