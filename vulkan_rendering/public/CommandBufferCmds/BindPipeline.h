//
// Created by miska on 10/9/2024.
//

#ifndef BINDPIPELINE_H
#define BINDPIPELINE_H
#include "CommandBufferCmd.h"


class BindPipeline final : public CommandBufferCmd{
    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
public:
    explicit BindPipeline(VkPipelineBindPoint bindPoint) : bindPoint(bindPoint) {}

    void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) override;
};

inline void BindPipeline::execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) {
    vkCmdBindPipeline(commandBuffer, bindPoint, context.vulkanPipeline);
}


#endif //BINDPIPELINE_H
