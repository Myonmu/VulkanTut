//
// Created by miska on 2024/02/10.
//

#pragma once


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

