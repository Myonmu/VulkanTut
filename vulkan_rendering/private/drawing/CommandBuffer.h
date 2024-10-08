//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-buffer-allocation
//

#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class CommandBuffer : public VulkanResource<VkCommandBuffer>{
public:
    explicit CommandBuffer(VulkanAppContext& context);
    ~CommandBuffer() override;
};



#endif //COMMANDBUFFER_H
