//
// Created by miska on 2024/08/04.
// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-buffer-allocation
//

#pragma once

#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "QueueContext.h"


enum class QueueFamily : uint32_t;
struct DeviceContext;

class CommandBuffer : public VulkanResource<VkCommandBuffer, DeviceContext>{
    QueueFamily queueFamily;
public:
    explicit CommandBuffer(DeviceContext& context, QueueFamily queueFamily);
    ~CommandBuffer() override;
    void executeImmediate();
};

