//
// Created by miska on 2024/10/21.
//

#pragma once
#include <DescriptorSets.h>
#include <map>
#include <VulkanPipeline.h>

#include "DescriptorPoolAllocator.h"
#include "ShaderReflectionResult.h"


struct DescriptorContext;
struct DeviceContext;

class Material {

    ShaderReflectionResult combinedReflectionResult{};
    std::map<uint32_t, std::unique_ptr<DescriptorSetLayout>> descriptorSetLayouts;
    std::unique_ptr<DescriptorAllocator> descriptorAllocator;
    std::unique_ptr<PipelineLayout> pipelineLayout;
    std::unique_ptr<VulkanPipeline> pipeline;


    Material(DeviceContext& ctx, std::vector<Shader> shaders,  RenderPass& renderPass);
    ~Material();
};
