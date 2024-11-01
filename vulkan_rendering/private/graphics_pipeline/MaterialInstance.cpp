//
// Created by Tereza on 10/29/2024.
//

#include "MaterialInstance.h"

#include <TextureImage.h>
#include <UniformBufferObject.h>

#include "DeviceContext.h"
#include "Material.h"
#include "UnifiedTexture2D.h"
#include "UniformBufferGroup.h"

MaterialInstance::MaterialInstance(Material &material) : srcMaterial(material), ctx(material.ctx) {
    for (auto &[id, layout]: material.descriptorSetLayouts) {
        auto result = material.descriptorAllocator->allocate(*layout);
        descriptorSets.emplace(id, std::move(result));
    }
}

MaterialInstance::~MaterialInstance() = default;

DescriptorSets &MaterialInstance::getDescriptorSet(const uint32_t setId) const {
    return *descriptorSets.at(setId);
}


void MaterialInstance::updateDescriptorSet(uint32_t setId, uint32_t i) {
    descriptorWriter.updateSet(ctx.getLogicalDevice(), getDescriptorSet(setId)[i]);
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

void MaterialInstance::setUBO(uint32_t binding, uint32_t i, UniformBufferGroup &ubo) {
    auto &buffer = ubo[i];
    descriptorWriter.writeBuffer(binding, buffer, sizeof(UniformBufferObject), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
}
