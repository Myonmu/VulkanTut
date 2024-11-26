//
// Created by miska on 2024/10/21.
//

#pragma once
#include <DescriptorSet.h>
#include <map>
#include <VulkanPipeline.h>

#include "ContextMacros.h"
#include "DescriptorPoolAllocator.h"
#include "DescriptorWriter.h"
#include "ObjectHierarchy.h"
#include "Shader.h"
#include "DescriptorSetLayout.h"


class PerFrameBufferGroup;
class TextureImage;
class UnifiedTexture2D;
class MaterialInstance;
struct DeviceContext;

//-----------------------------------MATERIAL------------------------------------------

class Material : public ObjectNode {
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
    DeviceContext &ctx;
    ShaderReflectionResult combinedReflectionResult{};
    std::map<uint32_t, std::unique_ptr<DescriptorSetLayout> > descriptorSetLayouts;
    std::unique_ptr<DescriptorAllocator> descriptorAllocator;

    CTX_PROPERTY(VulkanPipeline, pipeline)
    //TODO: Pipeline layout can be shared between materials (pipelines) if compatible
    CTX_PROPERTY(PipelineLayout, pipelineLayout)

public:
    Material(DeviceContext &ctx, std::vector<Shader> &shaders, RenderPass &renderPass, uint32_t subpassId,
             bool buildPipelineImmediately = true);

    ~Material() override = default;

    MaterialInstance &createInstance();
    void buildPipeline();

    DescriptorSetLayout &getDescriptorSetLayout(uint32_t setId) {
        return *descriptorSetLayouts[setId];
    }

    friend class MaterialInstance;
};


//-----------------------------------MATERIAL INSTANCE------------------------------------------


class MaterialInstance : public ObjectNode {
    Material &srcMaterial;
    DeviceContext &ctx;
    DescriptorWriter descriptorWriter{};

public:
    std::map<uint32_t, std::unique_ptr<DescriptorSet> > descriptorSets;

    explicit MaterialInstance(Material &material);

    ~MaterialInstance() override;

    friend class Material;


    void updateDescriptorSet(uint32_t setId);

    void setCombinedImageSampler(uint32_t binding, const UnifiedTexture2D &unifiedT2d, const TextureSampler &sampler);

    void setCombinedImageSampler(uint32_t binding, const TextureImage &textureImage, const TextureSampler &sampler,
                                 const ImageView &imageView);

    DescriptorSet &getDescriptorSet(uint32_t setId) const;

    [[nodiscard]] PipelineLayout &getPipelineLayout() const;

    [[nodiscard]] VulkanPipeline &getPipeline() const;

    Material &getMaterial() const { return srcMaterial; }
};
