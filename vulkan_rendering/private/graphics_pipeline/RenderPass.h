//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_RENDERPASS_H
#define VULKANTUT_RENDERPASS_H


#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>

struct PipelineContext;
class RenderPass : public VulkanResource<VkRenderPass, PipelineContext>{
public:
    explicit RenderPass(PipelineContext& context, VkFormat format);
    ~RenderPass() override;
};


#endif //VULKANTUT_RENDERPASS_H
