//
// Created by miska on 2024/02/11.
//

#pragma once



#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>

struct DeviceContext;
class RenderPass : public VulkanResource<VkRenderPass, DeviceContext>{
public:
    explicit RenderPass(DeviceContext& context, VkFormat format);
    ~RenderPass() override;
};

