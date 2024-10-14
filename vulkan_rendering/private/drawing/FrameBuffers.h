//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Framebuffers
//

#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct WindowContext;

class FrameBuffers: public VulkanResource<std::vector<VkFramebuffer>, WindowContext>{
public:
    explicit FrameBuffers(WindowContext& context);
    ~FrameBuffers() override;
    void recreate();
private:
    void createFrameBuffers();
    void destroyFrameBuffers();
};



#endif //FRAMEBUFFERS_H
