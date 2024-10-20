//
// Created by miska on 2024/10/10.
//

#include "TextureAnisotropyInfo.h"

#include <VulkanAppContext.h>
#include <optional>

#include "DeviceContext.h"

std::optional<bool> TextureAnisotropyInfo::supportsAnisotropy{};
std::optional<float> TextureAnisotropyInfo::supportedMaxAnisotropy{};

void TextureAnisotropyInfo::queryAnisotropyInfo(const DeviceContext& ctx) {
    if(supportsAnisotropy.has_value())return;

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(ctx.get_physicalDevice(), &supportedFeatures);
    supportsAnisotropy = supportedFeatures.samplerAnisotropy;
    if(!supportsAnisotropy) {
        supportedMaxAnisotropy = 1.0f;
        return;
    }
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(ctx.get_physicalDevice(), &properties);
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
