//
// Created by miska on 2024/10/10.
//

#ifndef TEXTUREMIPMAPINFO_H
#define TEXTUREMIPMAPINFO_H
#include <vulkan/vulkan_core.h>


struct TextureMipmapInfo {
    VkSamplerMipmapMode mipmapMode{};
    float mipmapBias{};
    float minLod{};
    float maxLod{};

    const static TextureMipmapInfo DEFAULT;
};



#endif //TEXTUREMIPMAPINFO_H
