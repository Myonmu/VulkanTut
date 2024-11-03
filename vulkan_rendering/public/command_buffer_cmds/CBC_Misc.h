//
// Created by miska on 10/9/2024.
//

#pragma once
#include <Buffer.h>
#include <CommandBuffer.h>
#include <CommandBufferRecorder.h>
#include <TextureImage.h>

#include "CommandBufferCmd.h"
#include "RenderPassRecorder.h"


class CBC_Misc final : public CommandBufferCmd {
    Buffer &src;
    Buffer &dst;
    VkDeviceSize srcOffset;
    VkDeviceSize dstOffset;
    VkDeviceSize size;

public:
    CBC_Misc(Buffer &src, Buffer &dst, VkDeviceSize srcOffset,
               VkDeviceSize dstOffset, VkDeviceSize size)
        : src(src),
          dst(dst),
          srcOffset(srcOffset),
          dstOffset(dstOffset),
          size(size) {
    };

    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = srcOffset;
        copyRegion.dstOffset = dstOffset;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer.getRaw(), src, dst, 1, &copyRegion);
    }
};

class CopyBufferToImage final : public CommandBufferCmd {
    const TextureImage &image;
    const Buffer &buffer;
    uint32_t width, height;

public:
    CopyBufferToImage(const Buffer &buffer, const TextureImage &image, uint32_t width, uint32_t height) : image(image),
        buffer(buffer), width(width), height(height) {
    }

    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );
    }
};


class EnqueueRecorder : public CommandBufferCmd {
    CommandBufferRecorder &recorder;

public:
    explicit EnqueueRecorder(CommandBufferRecorder &recorder): recorder(recorder) {
    }
    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo) override {
        recorder.recordCommandBuffer(commandBuffer, context, frameInfo);
    }
};


class EnqueueRenderPass final : public CommandBufferCmd{
    RenderPassRecorder& recorder;
public:
    explicit EnqueueRenderPass(RenderPassRecorder& recorder): recorder(recorder) {

    }
    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo) override {
        recorder.recordRenderPass(commandBuffer, context,  frameInfo);
    }
};


class TransitionImageLayout final : public CommandBufferCmd {
    TextureImage &image;
    VkFormat format;
    VkImageLayout oldLayout;
    VkImageLayout newLayout;

public:
    TransitionImageLayout(TextureImage &image, const VkFormat format, const VkImageLayout oldLayout,
                          const VkImageLayout newLayout) : image(image), format(format), oldLayout(oldLayout),
                                                           newLayout(newLayout) {
    }

    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo)
    override {
        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
            newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                   newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
    }
};
