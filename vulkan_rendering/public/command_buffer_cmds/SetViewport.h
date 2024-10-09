//
// Created by miska on 2024/10/08.
//

#ifndef SETVIEWPORT_H
#define SETVIEWPORT_H
#include <CommandBuffer.h>
#include <VulkanAppContext.h>

#include "CommandBufferCmd.h"


class SetViewport final: public CommandBufferCmd{
public:
    void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) override;
};

inline void SetViewport::execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) {
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(context.swapChain.swapChainExtent.width);
    viewport.height = static_cast<float>(context.swapChain.swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0,1,&viewport);
}


#endif //SETVIEWPORT_H
