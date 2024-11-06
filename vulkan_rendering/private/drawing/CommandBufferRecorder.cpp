//
// Created by miska on 2024/08/04.
//

#include "CommandBufferRecorder.h"
#include <VulkanAppContext.h>

#include "CommandBuffer.h"

CommandBufferRecorder::CommandBufferRecorder(bool isSubRecorder): isSubRecorder(isSubRecorder) {

};

CommandBufferRecorder::CommandBufferRecorder(const VkCommandBufferUsageFlags flags, bool isSubRecorder)
: flags(flags), isSubRecorder(isSubRecorder) {
}


void CommandBufferRecorder::beginRecordCommandBuffer(const CommandBuffer &commandBuffer) const {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags;
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

void CommandBufferRecorder::recordCommandBuffer(const CommandBuffer &commandBuffer, const DeviceContext &context,
                                                const FrameInfo &frameInfo) const {
    if(!isSubRecorder) {
        beginRecordCommandBuffer(commandBuffer);
    }

    if (recordFunc != nullptr) {
        recordFunc(commandBuffer, context, frameInfo);
    } else {
        for (const auto &cmd: commands) {
            cmd->execute(commandBuffer, context, frameInfo);
        }
    }

    if(!isSubRecorder) {
        endRecordCommandBuffer(commandBuffer);
    }
}

CommandBufferRecorder::~CommandBufferRecorder() {
    for (const auto &cmd: commands) {
        delete cmd;
    }
}

void CommandBufferRecorder::setRecordFunc(const RecordFunc &f) {
    recordFunc = f;
}

void CommandBufferRecorder::clear() {
    commands.clear();
}
