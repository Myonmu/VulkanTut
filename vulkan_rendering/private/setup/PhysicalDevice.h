//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_PHYSICALDEVICE_H
#define VULKANTUT_PHYSICALDEVICE_H

#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"

struct DeviceContext;

class PhysicalDevice: public VulkanResource<VkPhysicalDevice, DeviceContext>{
public:
    explicit PhysicalDevice(DeviceContext& context);
private:
    void pickPhysicalDevice(DeviceContext& context);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device, DeviceContext& context);
    static int rateDeviceSuitability(VkPhysicalDevice device, DeviceContext& context);
};


#endif //VULKANTUT_PHYSICALDEVICE_H
