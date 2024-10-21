//
// Created by miska on 2024/10/10.
//

#pragma once

#include <vulkan/vulkan_core.h>


struct TextureMipmapInfo {
    VkSamplerMipmapMode mipmapMode{};
    float mipmapBias{};
    float minLod{};
    float maxLod{};

    const static TextureMipmapInfo DEFAULT;
};

