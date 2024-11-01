//
// Created by miska on 10/10/2024.
//

#include "TextureSampler.h"
#include "DeviceContext.h"

const TextureAddressMode TextureAddressMode::REPEAT = {
    VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT
};


const TextureAddressMode TextureAddressMode::CLAMP_TO_BORDER = {
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
};

const TextureAddressMode TextureAddressMode::CLAMP_TO_EDGE = {
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
};

const TextureAddressMode TextureAddressMode::MIRRORED_REPEAT = {
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT
};

const TextureAddressMode TextureAddressMode::MIRROR_CLAMP_TO_EDGE = {
    VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
    VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
};

std::optional<bool> TextureAnisotropyInfo::supportsAnisotropy{};
std::optional<float> TextureAnisotropyInfo::supportedMaxAnisotropy{};

void TextureAnisotropyInfo::queryAnisotropyInfo(VkPhysicalDevice device) {
    if(supportsAnisotropy.has_value())return;

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    supportsAnisotropy = supportedFeatures.samplerAnisotropy;
    if(!supportsAnisotropy) {
        supportedMaxAnisotropy = 1.0f;
        return;
    }
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(device, &properties);
    supportedMaxAnisotropy = properties.limits.maxSamplerAnisotropy;

}

bool TextureAnisotropyInfo::isAnisotropySupported() {
    return supportsAnisotropy.value();
}

float TextureAnisotropyInfo::getMaxAnisotropy() {
    return supportedMaxAnisotropy.value();
}

TextureAnisotropyInfo TextureAnisotropyInfo::getAutoAnisotropyInfo() {
    return TextureAnisotropyInfo{isAnisotropySupported(), getMaxAnisotropy()};
}

const TextureFilterMode TextureFilterMode::LINEAR = {VK_FILTER_LINEAR, VK_FILTER_LINEAR};

const TextureFilterMode TextureFilterMode::NEAREST = {VK_FILTER_NEAREST, VK_FILTER_NEAREST};

const TextureMipmapInfo TextureMipmapInfo::DEFAULT{
    VK_SAMPLER_MIPMAP_MODE_LINEAR, 0.0f, 0.0f, 0.0f
};

TextureSampler::TextureSampler(DeviceContext &ctx,
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

    if (vkCreateSampler(ctx.getLogicalDevice(), &samplerInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }

}

TextureSampler::~TextureSampler() {
    vkDestroySampler(ctx.getLogicalDevice(), resource, nullptr);
}
