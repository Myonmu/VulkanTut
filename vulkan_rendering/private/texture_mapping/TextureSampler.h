//
// Created by miska on 10/10/2024.
//

#pragma once

#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct DeviceContext;
struct TextureMipmapInfo;
struct TextureCompareInfo;
struct TextureAnisotropyInfo;
struct TextureFilterMode;
struct TextureAddressMode;

class TextureSampler final : public VulkanResource<VkSampler, DeviceContext> {
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

