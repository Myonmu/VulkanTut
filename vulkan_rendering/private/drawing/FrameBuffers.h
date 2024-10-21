//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Framebuffers
//

#pragma once

#include <RenderPass.h>
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class RenderPass;
struct WindowContext;

class FrameBuffers: public VulkanResource<std::vector<VkFramebuffer>, WindowContext>{
public:
    explicit FrameBuffers(WindowContext& context, const RenderPass& renderPass);
    ~FrameBuffers() override;
    void recreate();
private:
    // saved reference to be used during recreation
    const RenderPass& renderPass;
    void createFrameBuffers(const RenderPass& renderPass);
    void destroyFrameBuffers();
};


