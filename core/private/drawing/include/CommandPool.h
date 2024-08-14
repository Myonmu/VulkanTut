//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-pools
//

#ifndef COMMANDPOOL_H
#define COMMANDPOOL_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class CommandPool: public VulkanResource<VkCommandPool> {
public:
    explicit CommandPool(VulkanAppContext& context);
    ~CommandPool() override;
};



#endif //COMMANDPOOL_H
