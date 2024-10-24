//
// Created by miska on 2024/02/11.
//

#pragma once

#include "VulkanResource.h"
#include "ShaderModule.h"
#include <vulkan/vulkan_core.h>
class SwapChain;
class RenderPass;
class PipelineLayout;
class PipelineContext;

// Per-object rendering pipeline
class VulkanPipeline : public VulkanResource<VkPipeline, DeviceContext> {
public:
    explicit VulkanPipeline(
        DeviceContext &context,
        const std::vector<Shader> &shaders,
        const PipelineLayout &layout,
        const RenderPass &renderPass
    );

    ~VulkanPipeline() override;

private:
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    std::vector<std::unique_ptr<ShaderModule> > shaderModules;
};
