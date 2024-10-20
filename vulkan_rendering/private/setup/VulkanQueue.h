//
// Created by miska on 2024/10/19.
//

#ifndef VULKANQUEUE_H
#define VULKANQUEUE_H
#include <VulkanResource.h>

#include "vulkan/vulkan_core.h"


struct DeviceContext;

class VulkanQueue: public VulkanResource<VkQueue, DeviceContext> {
public:
    VulkanQueue(DeviceContext& ctx, uint32_t queueFamilyIndex, uint32_t queueIndex);
    ~VulkanQueue() override;
};



#endif //VULKANQUEUE_H
