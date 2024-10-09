//
// Created by miska on 2024/10/09.
//

#ifndef COPYBUFFERTOIMAGE_H
#define COPYBUFFERTOIMAGE_H
#include <Buffer.h>
#include <TextureImage.h>

#include "CommandBufferCmd.h"
#include "TransitionImageLayout.h"


class CopyBufferToImage final : public CommandBufferCmd {
    const TextureImage &image;
    const Buffer &buffer;
    uint32_t width, height;

public:
    CopyBufferToImage(const Buffer &buffer, const TextureImage &image, uint32_t width, uint32_t height) : image(image),
        buffer(buffer), width(width), height(height) {
    }

    void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context,
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


#endif //COPYBUFFERTOIMAGE_H
