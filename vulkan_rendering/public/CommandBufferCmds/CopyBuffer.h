//
// Created by miska on 10/9/2024.
//

#ifndef COPYBUFFER_H
#define COPYBUFFER_H
#include <Buffer.h>
#include <CommandBuffer.h>

#include "CommandBufferCmd.h"


class CopyBuffer final: public CommandBufferCmd{
    Buffer& src;
    Buffer& dst;
    VkDeviceSize srcOffset;
    VkDeviceSize dstOffset;
    VkDeviceSize size;
public:
    CopyBuffer(Buffer& src, Buffer& dst, VkDeviceSize srcOffset,
        VkDeviceSize dstOffset, VkDeviceSize size)
    :src(src),
    dst(dst),
    srcOffset(srcOffset),
    dstOffset(dstOffset),
    size(size) {};
    void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) override;
};

inline void CopyBuffer::execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) {
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = srcOffset;
    copyRegion.dstOffset = dstOffset;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer.getRaw(), src, dst, 1, &copyRegion);
}



#endif //COPYBUFFER_H
