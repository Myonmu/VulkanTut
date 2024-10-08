//
// Created by miska on 2024/08/04.
//

#include "CommandBufferRecorder.h"
#include <VulkanAppContext.h>

#include "CommandBuffer.h"
#include "FrameInfo.h"

void CommandBufferRecorder::beginRecordCommandBuffer(const CommandBuffer &commandBuffer) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer..");
    }
}

void CommandBufferRecorder::endRecordCommandBuffer(const CommandBuffer &commandBuffer) {
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
}

void CommandBufferRecorder::recordCommandBuffer(const VulkanAppContext &context,
                                                const CommandBuffer &commandBuffer, const FrameInfo &frameInfo) {
    beginRecordCommandBuffer(commandBuffer);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = context.renderPass;
    renderPassInfo.framebuffer = context.frameBuffers.getRaw()[frameInfo.imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = context.swapChain.swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, context.vulkanPipeline);

    for (const auto &cmd: commands) {
        cmd->execute(commandBuffer, context, frameInfo);
    }

    vkCmdEndRenderPass(commandBuffer);

    endRecordCommandBuffer(commandBuffer);
}

CommandBufferRecorder::~CommandBufferRecorder() {
    for (const auto &cmd: commands) {
        delete cmd;
    }
}
