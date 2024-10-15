//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-buffer-allocation
//

#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct DeviceContext;

class CommandBuffer : public VulkanResource<VkCommandBuffer, DeviceContext>{
public:
    explicit CommandBuffer(DeviceContext& context);
    ~CommandBuffer() override;
    void executeImmediate();
};



#endif //COMMANDBUFFER_H
