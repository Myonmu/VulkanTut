//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-pools
//

#pragma once

#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct DeviceContext;

class CommandPool: public VulkanResource<VkCommandPool, DeviceContext> {
public:
    explicit CommandPool(DeviceContext& context, uint32_t queueFamilyIndex,
        VkCommandPoolCreateFlagBits flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    ~CommandPool() override;
};


