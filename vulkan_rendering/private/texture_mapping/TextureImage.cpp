//
// Created by miska on 2024/09/02.
//

#include "TextureImage.h"

#include <VulkanAppContext.h>

#include "CBC_Misc.h"
#include "DeviceContext.h"
#include "FrameInfo.h"

TextureImage::TextureImage(DeviceContext &ctx,
                           const int &width, const int &height, const int &channels,
                           VkFormat textureFormat,
                           VkImageTiling tiling,
                           VkImageUsageFlags usage,
                           VkMemoryPropertyFlags memoryProperties
): VulkanResource(ctx),
   width(width),
   height(height), channels(channels), format(textureFormat),
   imageSize(width * height * channels),
   stagingBuffer(ctx,
                 imageSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VMA_MEMORY_USAGE_CPU_ONLY
                 //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
   ) {
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.tiling = tiling;
    imageInfo.format = textureFormat;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.flags = 0;

    if (vkCreateImage(ctx.getLogicalDevice(), &imageInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(ctx.getLogicalDevice(), resource, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memoryProperties);

    if (vkAllocateMemory(ctx.getLogicalDevice(), &allocInfo, nullptr, &textureImageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate memory!");
    }

    vkBindImageMemory(ctx.getLogicalDevice(), resource, textureImageMemory, 0);
}

TextureImage::TextureImage(DeviceContext &ctx, Texture2D &t2d) : TextureImage(
    ctx, t2d.getWidth(), t2d.getHeight(), 4, t2d.getFormat(),
    VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
    stagingBuffer.copyToBufferMemory(t2d.pixels, 0);
}


uint32_t TextureImage::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(ctx.get_physicalDevice(), &memProperties);
    for (auto i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & 1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type");
}

void TextureImage::stage() {
    CommandBuffer cmd{ctx, QueueFamily::QUEUE_FAMILY_GRAPHICS};
    CommandBufferRecorder recorder{};

    recorder.enqueueCommand<CopyBufferToImage>(stagingBuffer, *this,
                                               static_cast<uint32_t>(width),
                                               static_cast<uint32_t>(height)
    );

    recorder.recordCommandBuffer(cmd, ctx, FrameInfo::DONT_CARE);

    cmd.executeImmediate();
}

void TextureImage::transitionLayout(VkImageLayout newLayout) {
    CommandBuffer cmd{ctx, QueueFamily::QUEUE_FAMILY_GRAPHICS};
    CommandBufferRecorder recorder{};
    recorder.enqueueCommand<TransitionImageLayout>(*this,
                                                   format,
                                                   currentLayout,
                                                   newLayout);
    recorder.recordCommandBuffer(cmd, ctx, FrameInfo::DONT_CARE);
    cmd.executeImmediate();
    currentLayout = newLayout;
}

TextureImage::~TextureImage() {
    vkDestroyImage(ctx.getLogicalDevice(), resource, nullptr);
    vkFreeMemory(ctx.getLogicalDevice(), textureImageMemory, nullptr);
}
