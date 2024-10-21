//
// Created by miska on 2024/10/08.
//

#ifndef SETVIEWPORT_H
#define SETVIEWPORT_H
#include <CommandBuffer.h>
#include <VulkanAppContext.h>

#include "CommandBufferCmd.h"


class SetViewport final : public CommandBufferCmd {
public:
    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        auto& swapChain = context.get_windowContext_at(frameInfo.windowId).get_swapChain();
        auto [width, height] = swapChain.swapChainExtent;
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(width);
        viewport.height = static_cast<float>(height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    }
};


#endif //SETVIEWPORT_H
