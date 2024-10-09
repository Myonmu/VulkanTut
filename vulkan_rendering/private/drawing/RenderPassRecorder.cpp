//
// Created by miska on 10/9/2024.
//

#include "RenderPassRecorder.h"

#include <VulkanAppContext.h>

#include "FrameInfo.h"


RenderPassRecorder::RenderPassRecorder(RenderPass &renderPass) : renderPass(renderPass) {
}

RenderPassRecorder::~RenderPassRecorder() {
    for (const auto &cmd: commands) {
        delete cmd;
    }
}


void RenderPassRecorder::recordRenderPass(const CommandBuffer &commandBuffer, const VulkanAppContext &context,
                                          const FrameInfo &frameInfo) const {
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = context.frameBuffers.getRaw()[frameInfo.imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = context.swapChain.swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    for (const auto &cmd: commands) {
        cmd->execute(commandBuffer, context, frameInfo);
    }

    vkCmdEndRenderPass(commandBuffer);
}
