//
// Created by miska on 10/10/2024.
//

#pragma once

#include <optional>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "ObjectHierarchy.h"


struct DeviceContext;
struct TextureMipmapInfo;
struct TextureAnisotropyInfo;

struct TextureAddressMode {

    VkSamplerAddressMode u{};
    VkSamplerAddressMode v{};
    VkSamplerAddressMode w{};

    const static TextureAddressMode REPEAT;
    const static TextureAddressMode CLAMP_TO_EDGE;
    const static TextureAddressMode CLAMP_TO_BORDER;
    const static TextureAddressMode MIRRORED_REPEAT;
    const static TextureAddressMode MIRROR_CLAMP_TO_EDGE;
};

struct TextureAnisotropyInfo {
    bool enabled{VK_TRUE};
    float maxAnisotropy{1.0f};

    static TextureAnisotropyInfo getAutoAnisotropyInfo();
    static bool isAnisotropySupported();
    static float getMaxAnisotropy();
    static void queryAnisotropyInfo(VkPhysicalDevice device);
private:
    static std::optional<bool> supportsAnisotropy;
    static std::optional<float> supportedMaxAnisotropy;
};


struct TextureCompareInfo {
    bool enabled{VK_FALSE};
    VkCompareOp compareOp{VK_COMPARE_OP_ALWAYS};
};

struct TextureFilterMode {
    VkFilter min;
    VkFilter mag;

    const static TextureFilterMode LINEAR;
    const static TextureFilterMode NEAREST;
};


struct TextureMipmapInfo {
    VkSamplerMipmapMode mipmapMode{};
    float mipmapBias{};
    float minLod{};
    float maxLod{};

    const static TextureMipmapInfo DEFAULT;
};

class TextureSampler final : public VulkanResource<VkSampler, DeviceContext>, public ObjectNode {
public:
    explicit TextureSampler(DeviceContext &ctx,
                   TextureAddressMode addressMode,
                   TextureFilterMode filterMode,
                   TextureAnisotropyInfo anisotropyInfo,
                   TextureCompareInfo compareInfo,
                   TextureMipmapInfo mipmapInfo,
                   VkBorderColor borderColor,
                   bool unnormalizedCoords
    );

    ~TextureSampler() override;
};

