//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_RENDERPASS_H
#define VULKANTUT_RENDERPASS_H


#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>

struct DeviceContext;
class RenderPass : public VulkanResource<VkRenderPass, DeviceContext>{
public:
    explicit RenderPass(DeviceContext& context, VkFormat format);
    ~RenderPass() override;
};


#endif //VULKANTUT_RENDERPASS_H
