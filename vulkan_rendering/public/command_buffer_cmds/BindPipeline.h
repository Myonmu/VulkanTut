//
// Created by miska on 10/9/2024.
//

#ifndef BINDPIPELINE_H
#define BINDPIPELINE_H
#include <VulkanAppContext.h>
#include <VulkanPipeline.h>
#include <vulkan/vulkan_core.h>
#include "CommandBufferCmd.h"


class BindPipeline final : public CommandBufferCmd{
    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    VulkanPipeline& pipeline;
public:
    explicit BindPipeline(VulkanPipeline& pipeline, VkPipelineBindPoint bindPoint) : bindPoint(bindPoint), pipeline(pipeline) {}

    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo) override {
        vkCmdBindPipeline(commandBuffer, bindPoint, pipeline);
    }
};


#endif //BINDPIPELINE_H
