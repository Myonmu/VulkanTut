//
// Created by miska on 10/10/2024.
//

#pragma once

#include <vulkan/vulkan_core.h>


struct TextureFilterMode {
    VkFilter min;
    VkFilter mag;

    const static TextureFilterMode LINEAR;
    const static TextureFilterMode NEAREST;
};
