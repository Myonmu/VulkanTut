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

    /* The descriptors can get complicated here.
     *
     * The shaders can declare usage of multiple descriptor sets.
     * However, it doesn't mean these descriptors sets have different layouts.
     *
     * Descriptor pools do not really care the bindings of the descriptor set layouts,
     * therefore the pools are determined by on type-count pairs.
     *
     *
     *
     */

    // reflection data from all shader stages
    ShaderReflectionResult combinedReflectionResult{};
    std::map<uint32_t, std::unique_ptr<DescriptorSetLayout>> descriptorSetLayouts;
    std::map<uint32_t, std::unique_ptr<DescriptorAllocator>> descriptorAllocator;
    std::unique_ptr<PipelineLayout> pipelineLayout;
    std::unique_ptr<VulkanPipeline> pipeline;


    Material(DeviceContext& ctx, std::vector<Shader> shaders,  RenderPass& renderPass);
    ~Material();
};
