//
// Created by miska on 10/10/2024.
//

#ifndef TEXTURESAMPLER_H
#define TEXTURESAMPLER_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct DeviceContext;
struct TextureMipmapInfo;
struct TextureCompareInfo;
struct TextureAnisotropyInfo;
struct TextureFilterMode;
struct TextureAddressMode;

class TextureSampler : public VulkanResource<VkSampler, DeviceContext> {
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


#endif //TEXTURESAMPLER_H
