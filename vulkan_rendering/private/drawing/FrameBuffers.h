//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Framebuffers
//

#pragma once

#include <RenderPass.h>
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "Polymorphism.h"


class RenderPass;
struct WindowContext;

/**
* VkFramebuffer wrapper.
* Contains N frame buffers where N = swap chain present attachment count.
* Allocated per-render-pass
*/
class FrameBuffer: public VulkanResource<std::vector<VkFramebuffer>, WindowContext>{
public:
    explicit FrameBuffer(WindowContext& context, const RenderPass& renderPass);
    ~FrameBuffer() override;
    void recreate();
    // saved reference to be used during recreation
    const RenderPass& renderPass;
    void createFrameBuffers(const RenderPass& renderPass);
    void destroyFrameBuffers();
};


