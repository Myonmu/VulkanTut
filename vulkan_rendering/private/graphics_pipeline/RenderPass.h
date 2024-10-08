//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_RENDERPASS_H
#define VULKANTUT_RENDERPASS_H


#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>

struct VulkanAppContext;
class RenderPass : public VulkanResource<VkRenderPass>{
public:
    explicit RenderPass(VulkanAppContext& context);
    ~RenderPass() override;
};


#endif //VULKANTUT_RENDERPASS_H
