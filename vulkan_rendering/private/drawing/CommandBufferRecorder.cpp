//
// Created by miska on 2024/08/04.
//

#include "CommandBufferRecorder.h"
#include <VulkanAppContext.h>

#include "CommandBuffer.h"
#include "FrameInfo.h"

CommandBufferRecorder::CommandBufferRecorder() = default;

CommandBufferRecorder::CommandBufferRecorder(const VkCommandBufferUsageFlags flags) :flags(flags){

}


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

void CommandBufferRecorder::recordCommandBuffer(const CommandBuffer &commandBuffer, const VulkanAppContext &context,
                                                const FrameInfo &frameInfo) const {
    beginRecordCommandBuffer(commandBuffer);

    for (const auto &cmd: commands) {
        cmd->execute(commandBuffer, context, frameInfo);
    }

    endRecordCommandBuffer(commandBuffer);
}

CommandBufferRecorder::~CommandBufferRecorder() {
    for (const auto &cmd: commands) {
        delete cmd;
    }
}
