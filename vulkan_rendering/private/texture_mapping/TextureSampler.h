//
// Created by miska on 10/10/2024.
//

#ifndef TEXTURESAMPLER_H
#define TEXTURESAMPLER_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class TextureFilterMode;
class TextureAddressMode;

class TextureSampler : public VulkanResource<VkSampler> {
public:
    TextureSampler(VulkanAppContext &ctx,
                   TextureAddressMode &addressMode,
                   TextureFilterMode &filterMode);

    ~TextureSampler() override;
};


#endif //TEXTURESAMPLER_H
