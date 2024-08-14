//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Framebuffers
//

#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class FrameBuffers: public VulkanResource<std::vector<VkFramebuffer>>{
public:
    explicit FrameBuffers(VulkanAppContext& context);
    ~FrameBuffers() override;
private:
    VulkanAppContext* context;
};



#endif //FRAMEBUFFERS_H
