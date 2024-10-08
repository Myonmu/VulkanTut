//
// Created by miska on 2024/10/08.
//

#ifndef SETSCISSORS_H
#define SETSCISSORS_H
#include <VulkanAppContext.h>

#include "CommandBufferCmd.h"

class SetScissors final: public CommandBufferCmd {
public:
    void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) override;
};

inline void SetScissors::execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) {
    VkRect2D scissor{};
    scissor.offset = {0,0};
    scissor.extent = context.swapChain.swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
#endif //SETSCISSORS_H
