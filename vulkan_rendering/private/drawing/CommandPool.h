//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-pools
//

#ifndef COMMANDPOOL_H
#define COMMANDPOOL_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct VulkanAppContext;

class CommandPool: public VulkanResource<VkCommandPool, VulkanAppContext> {
public:
    explicit CommandPool(VulkanAppContext& context);
    ~CommandPool() override;
};



#endif //COMMANDPOOL_H
