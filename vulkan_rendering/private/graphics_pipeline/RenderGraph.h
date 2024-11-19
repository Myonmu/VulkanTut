//
// Created by Tereza on 11/19/2024.
//

/* Adapted from:
 *  https://github.com/Themaister/Granite/blob/master/renderer/render_graph.hpp
 *
 *  primary article: https://themaister.net/blog/2017/08/15/render-graphs-and-vulkan-a-deep-dive/
 *  supplementary article: https://poniesandlight.co.uk/reflect/island_rendergraph_1/
 */

#pragma once
#include <vulkan/vulkan_core.h>


enum class AttachmentSizeMode {
    SWAPCHAIN_RELATIVE,
    ABSOLUTE,
    INPUT_RELATIVE
};

// "Virtual" attachment handle (not "Virtual Texture", just an attachment placeholder)
// Contains enough information to generate an alloc info
struct AttachmentDecl {
    AttachmentSizeMode sizeMode = AttachmentSizeMode::SWAPCHAIN_RELATIVE;
    float width = 1.f;
    float height = 1.f;
    float depth = 0.f;
    VkFormat format = VK_FORMAT_UNDEFINED;
    uint32_t samples = 1;
    uint32_t levels = 1;
    uint32_t layers = 1;
    VkImageUsageFlags usage = 0;
};

struct BufferDecl {
    VkDeviceSize size = 0;
    VkBufferUsageFlags usage = 0;
};

struct RenderGraphNode {
    
};


class RenderGraph {
};
