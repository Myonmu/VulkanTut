//
// Created by miska on 2024/10/21.
//

#pragma once
#include <map>
#include <VulkanPipeline.h>

#include "ShaderReflectionResult.h"


struct DescriptorContext;
struct DeviceContext;

class Material {
    std::vector<VkDescriptorSetLayout> vkLayouts;
    std::map<uint32_t, std::unique_ptr<DescriptorContext>> descriptorContexts;
    std::unique_ptr<PipelineLayout> pipelineLayout;
    std::unique_ptr<VulkanPipeline> pipeline;
    ShaderReflectionResult combinedReflectionResult{};

    Material(DeviceContext& ctx, std::vector<Shader> shaders,  RenderPass& renderPass);
    ~Material();
};
