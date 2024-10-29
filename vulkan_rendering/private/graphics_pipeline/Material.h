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
     * therefore the pools are determined by type-count pairs. For example, the shader code below:
     *   layout(set = 0, binding = 0) uniform {...} uboA;
     *   layout(set = 0, binding = 1) uniform {...} uboB;
     * declares 2 descriptors of type UNIFORM_BUFFER. Similarly,
     *   layout(set = 0, binding = 1) uniform{...} uboA;
     *   layout(set = 0, binding = 5) uniform{...} uboB;
     * is the same, 2 descriptors of type UNIFORM_BUFFER.
     * This means despite they can be drastically different, we can still use the same
     * Descriptor Pool to allocate them as long as they have the same type requirements
     *
     *
     *
     */

    // reflection data from all shader stages
    ShaderReflectionResult combinedReflectionResult{};
    std::map<uint32_t, std::unique_ptr<DescriptorSetLayout>> descriptorSetLayouts;
    std::unique_ptr<DescriptorAllocator> descriptorAllocator;
    //TODO: Pipeline layout can be shared between materials (pipelines) if compatible
    std::unique_ptr<PipelineLayout> pipelineLayout;
    std::unique_ptr<VulkanPipeline> pipeline;
    DeviceContext& ctx;
public:
    Material(DeviceContext& ctx, std::vector<Shader> shaders,  RenderPass& renderPass);
    ~Material();

    friend class MaterialInstance;
};
