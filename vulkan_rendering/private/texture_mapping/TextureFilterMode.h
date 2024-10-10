//
// Created by miska on 10/10/2024.
//

#ifndef TEXTUREFILTERMODE_H
#define TEXTUREFILTERMODE_H
#include <vulkan/vulkan_core.h>


class TextureFilterMode {
public:
    VkFilter min;
    VkFilter mag;

    const static TextureFilterMode LINEAR;
    const static TextureFilterMode NEAREST;
};



#endif //TEXTUREFILTERMODE_H
