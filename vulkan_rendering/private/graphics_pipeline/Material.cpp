//
// Created by miska on 2024/10/21.
//
#include "Material.h"
#include <DescriptorSetLayout.h>
#include <PipelineLayout.h>

#include "DeviceContext.h"
#include "Shader.h"
#include "UnifiedTexture2D.h"
#include "PerFrameBufferGroup.h"

Material::Material(DeviceContext& ctx, std::vector<Shader>& shaders, RenderPass& renderPass, uint32_t subpassId):
ctx(ctx)
{
    for (auto& shader: shaders) {
        combinedReflectionResult.merge(shader.reflectionResult);
    }

    descriptorAllocator = std::make_unique<DescriptorAllocator>(ctx);
    auto stats = combinedReflectionResult.getCountByType();
    std::vector<DescriptorAllocator::PoolSizeRatio> ratios;
    for (auto const&[type, count]: stats) {
        ratios.push_back({type, static_cast<float>(count)});
    }
    descriptorAllocator->init(100, ratios);

    std::vector<VkDescriptorSetLayout> vkLayouts;
    for(auto& [set, layouts] : combinedReflectionResult.descriptorSets) {
        descriptorSetLayouts[set] = std::make_unique<DescriptorSetLayout>(ctx, set, layouts);
        vkLayouts.push_back(descriptorSetLayouts[set]->getRaw());
    }
    pipelineLayout = std::make_unique<PipelineLayout>(ctx, vkLayouts);
    pipeline = std::make_unique<VulkanPipeline>(ctx, shaders, *pipelineLayout, renderPass, subpassId);
}

MaterialInstance &Material::createInstance() {
    return createChildObject<MaterialInstance>(*this);
}





MaterialInstance::MaterialInstance(Material &material) : srcMaterial(material), ctx(material.ctx) {
    for (auto &[id, layout]: material.descriptorSetLayouts) {
        if(layout->getAllocPolicy() != DescriptorAllocPolicy::PER_MATERIAL_INSTANCE) continue;
        auto result = material.descriptorAllocator->allocate(*layout);
        descriptorSets.emplace(id, std::move(result));
    }
}

MaterialInstance::~MaterialInstance() = default;

DescriptorSet &MaterialInstance::getDescriptorSet(const uint32_t setId) const {
    return *descriptorSets.at(setId);
}


void MaterialInstance::updateDescriptorSet(uint32_t setId) {
    descriptorWriter.updateSet(ctx.getLogicalDevice(), getDescriptorSet(setId));
}

void MaterialInstance::setCombinedImageSampler(uint32_t binding, const UnifiedTexture2D &unifiedT2d,
    const TextureSampler& sampler) {
    setCombinedImageSampler(binding, unifiedT2d.get_textureImage(), sampler, unifiedT2d.get_imageView() );
}


void MaterialInstance::setCombinedImageSampler(const uint32_t binding, const TextureImage &textureImage,
                                               const TextureSampler &sampler, const ImageView &imageView) {
    descriptorWriter.writeImage(binding, imageView, sampler, textureImage.getCurrentLayout(),
                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
}

PipelineLayout &MaterialInstance::getPipelineLayout() const {
    return srcMaterial.get_pipelineLayout();
}

VulkanPipeline &MaterialInstance::getPipeline() const {
    return srcMaterial.get_pipeline();
}
