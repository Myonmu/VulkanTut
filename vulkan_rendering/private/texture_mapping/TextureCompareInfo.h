//
// Created by miska on 2024/10/10.
//

#pragma once

#include <vulkan/vulkan_core.h>


struct TextureCompareInfo {
    bool enabled{VK_FALSE};
    VkCompareOp compareOp{VK_COMPARE_OP_ALWAYS};
};


