//
// Created by miska on 2024/02/10.
//

#ifndef VULKANTUT_VULKANSURFACE_H
#define VULKANTUT_VULKANSURFACE_H

#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"

struct VulkanAppContext;

class VulkanSurface : public VulkanResource<VkSurfaceKHR>{
public:
    explicit VulkanSurface(VulkanAppContext &context);
    ~VulkanSurface() override;
private:
    VulkanAppContext* ctx;
    void createSurface(VulkanAppContext& context);
};

#endif //VULKANTUT_VULKANSURFACE_H
