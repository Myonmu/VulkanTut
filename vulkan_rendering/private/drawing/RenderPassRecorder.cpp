//
// Created by miska on 10/9/2024.
//

#include "RenderPassRecorder.h"

#include <VulkanAppContext.h>

#include "DeviceContext.h"
#include "FrameInfo.h"
#include "WindowContext.h"


RenderPassRecorder::RenderPassRecorder(RenderPass &renderPass) : renderPass(renderPass) {
}

RenderPassRecorder::~RenderPassRecorder() = default;


void RenderPassRecorder::recordRenderPass(const CommandBuffer &commandBuffer, const DeviceContext &context,
                                          const FrameInfo &frameInfo) const {
    auto& window = context.get_windowContext_at(frameInfo.windowId);
    auto& frameBuffers = window.get_frameBuffers();
    auto& swapChain = window.get_swapChain();
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = frameBuffers.getRaw()[frameInfo.imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain.swapChainExtent;


    // TODO: Move clear values to ColorAttachment and DepthAttachment classes
    // TODO: Generate this based on render pass
    std::array<VkClearValue, 3> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};
    clearValues[2].color = {{0.0f, 0.0f, 0.0f, 1.0f}};


    renderPassInfo.clearValueCount = clearValues.size();
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    for (const auto &cmd: commands) {
        cmd->execute(commandBuffer, context, frameInfo);
    }

    vkCmdEndRenderPass(commandBuffer);
}

void RenderPassRecorder::clear() {
    commands.clear();
}

