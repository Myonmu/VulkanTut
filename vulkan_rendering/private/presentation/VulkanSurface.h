//
// Created by miska on 2024/02/10.
//

#ifndef VULKANTUT_VULKANSURFACE_H
#define VULKANTUT_VULKANSURFACE_H

#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"

struct WindowContext;

class VulkanSurface : public VulkanResource<VkSurfaceKHR, WindowContext>{
public:
    explicit VulkanSurface(WindowContext &context);
    ~VulkanSurface() override;
private:
    void createSurface(WindowContext& context);
};

#endif //VULKANTUT_VULKANSURFACE_H
