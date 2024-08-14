//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_VULKANPIPELINE_H
#define VULKANTUT_VULKANPIPELINE_H


#include "VulkanResource.h"
#include "ShaderModule.h"
#include <vulkan/vulkan_core.h>
class VulkanPipeline : public VulkanResource<VkPipeline>{
public:
    explicit VulkanPipeline(VulkanAppContext &context);
    ~VulkanPipeline() override;
private:
    std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };
    ShaderModule frag;
    ShaderModule vert;
};


#endif //VULKANTUT_VULKANPIPELINE_H
