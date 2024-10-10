//
// Created by miska on 10/10/2024.
//

#include "TextureSampler.h"

#include <VulkanAppContext.h>

#include "TextureAddressMode.h"
#include "TextureAnisotropyInfo.h"
#include "TextureCompareInfo.h"
#include "TextureFilterMode.h"
#include "TextureMipmapInfo.h"

TextureSampler::TextureSampler(VulkanAppContext &ctx,
                               const TextureAddressMode addressMode,
                               const TextureFilterMode filterMode,
                               const TextureAnisotropyInfo anisotropyInfo,
                               const TextureCompareInfo compareInfo,
                               const TextureMipmapInfo mipmapInfo,
                               const VkBorderColor borderColor,
                               const bool unnormalizedCoords) : VulkanResource(ctx) {

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.addressModeU = addressMode.u;
    samplerInfo.addressModeV = addressMode.v;
    samplerInfo.addressModeW = addressMode.w;

    samplerInfo.borderColor = borderColor;
    samplerInfo.unnormalizedCoordinates = unnormalizedCoords;

    samplerInfo.anisotropyEnable = anisotropyInfo.enabled;
    samplerInfo.maxAnisotropy = anisotropyInfo.maxAnisotropy;

    samplerInfo.compareEnable = compareInfo.enabled;
    samplerInfo.compareOp = compareInfo.compareOp;

    samplerInfo.mipmapMode = mipmapInfo.mipmapMode;
    samplerInfo.mipLodBias = mipmapInfo.mipmapBias;
    samplerInfo.minLod = mipmapInfo.minLod;
    samplerInfo.maxLod = mipmapInfo.maxLod;

    samplerInfo.minFilter = filterMode.min;
    samplerInfo.magFilter = filterMode.mag;

    if (vkCreateSampler(ctx.logicalDevice, &samplerInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }

}

TextureSampler::~TextureSampler() {
    vkDestroySampler(ctx.logicalDevice, resource, nullptr);
}
