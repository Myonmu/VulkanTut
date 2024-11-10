//
// Created by miska on 2024/02/08.
//

#pragma once


#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"
#include <vector>

struct DeviceContext;

class PhysicalDevice: public VulkanResource<VkPhysicalDevice, DeviceContext>{
public:
    explicit PhysicalDevice(DeviceContext& context);
    [[nodiscard]] uint32_t getApiVersion() const;
    VkFormat findSupportedImageFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                 VkFormatFeatureFlags features) const;
private:
    void pickPhysicalDevice(DeviceContext& context);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device, DeviceContext& context);
    static int rateDeviceSuitability(VkPhysicalDevice device, DeviceContext& context);
};

