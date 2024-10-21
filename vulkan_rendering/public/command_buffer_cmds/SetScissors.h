//
// Created by miska on 2024/10/08.
//

#ifndef SETSCISSORS_H
#define SETSCISSORS_H
#include <VulkanAppContext.h>

#include "CommandBufferCmd.h"

class SetScissors final : public CommandBufferCmd {
public:
    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = context.get_windowContext_at(frameInfo.windowId).get_swapChain().swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }
};

#endif //SETSCISSORS_H
