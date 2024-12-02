//
// Created by miska on 2024/09/02.
//

#include "TextureImage.h"

#include <VulkanAppContext.h>

#include "CBC_Misc.h"
#include "DeviceContext.h"
#include "FrameInfo.h"
#include "VmaInstance.h"

// must be multiple of 2, max 64
TextureImageInfo &TextureImageInfo::setSampleCount(uint32_t count) {
    switch (count) {
        case 1: msaaSamples = VK_SAMPLE_COUNT_1_BIT;
            break;
        case 2: msaaSamples = VK_SAMPLE_COUNT_2_BIT;
            break;
        case 4: msaaSamples = VK_SAMPLE_COUNT_4_BIT;
            break;
        case 8: msaaSamples = VK_SAMPLE_COUNT_8_BIT;
            break;
        case 16: msaaSamples = VK_SAMPLE_COUNT_16_BIT;
            break;
        case 32: msaaSamples = VK_SAMPLE_COUNT_32_BIT;
            break;
        case 64: msaaSamples = VK_SAMPLE_COUNT_64_BIT;
            break;
        default:
            throw std::invalid_argument("sample count must be 1, 2, 4, 8, 16, 32, or 64");
    }
    if (count > 1) {
        implicitUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    return *this;
}

TextureImageInfo &TextureImageInfo::setMipLevels(uint32_t count) {
    if (count <= 1) {
        mipLevels = 1;
        //remove implicit transfer source (assuming mip level is the only implicit usage that requires transfer source)
        implicitUsageFlags &= ~VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    } else {
        mipLevels = count;
        implicitUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    return *this;
}

TextureImageInfo &TextureImageInfo::setMaxMipLevels() {
    mipLevels = dimensions.getLevels();
    return *this;
}


TextureImageInfo &TextureImageInfo::setLayers(uint32_t count) {
    layers = count <= 1 ? 1 : count;
    return *this;
}

TextureImageInfo &TextureImageInfo::isInputAttachment() {
    usage |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
    return *this;
}

TextureImageInfo &TextureImageInfo::isTransientAttachment() {
    usage |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    return *this;
}

uint32_t TextureImageInfo::getSize() const {
    // TODO: array type? haven't tried to allocate one
    return dimensions.width * dimensions.height * dimensions.depth * channels;
}

TextureImageInfo::operator VkImageCreateInfo() const {
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = type;
    imageInfo.extent.width = dimensions.width;
    imageInfo.extent.height = dimensions.height;
    imageInfo.extent.depth = dimensions.depth;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = layers;
    imageInfo.samples = msaaSamples;
    imageInfo.usage = usage | implicitUsageFlags;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.tiling = tiling;
    imageInfo.format = format;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.flags = flags;
    return imageInfo;
}

bool TextureImageInfo::operator==(const TextureImageInfo &other) const {
    return format == other.format &&
        dimensions == other.dimensions &&
            mipLevels == other.mipLevels &&
                layers == other.layers;
}

bool TextureImageInfo::operator!=(const TextureImageInfo &other) const {
    return !(*this == other);
}


void TextureImage::create() {
    VkImageCreateInfo createInfo = info;
    VmaAllocationCreateInfo allocInfo = info;

    auto &vmaAllocator = ctx.get_vma();
    auto result = vmaCreateImage(vmaAllocator, &createInfo, &allocInfo, &resource, &vmaAllocation, nullptr);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }
}


TextureImage::TextureImage(DeviceContext &ctx, TextureImageInfo &info, StagingBufferMode stagingBufferMode)
    : VulkanResource(ctx), info(info), imageSize(info.getSize()) {
    if (stagingBufferMode != StagingBufferMode::NO_STAGING_BUFFER) {
        auto stagingBufferInfo = BufferInfo::createStagingBufferInfo(imageSize,
                                                                     stagingBufferMode ==
                                                                     StagingBufferMode::PERSISTENT);
        stagingBuffer = std::make_unique<Buffer>(ctx, stagingBufferInfo);
    }
    create();

    /* replaced with VMA
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
    */
}

TextureImage::TextureImage(DeviceContext &ctx,
                           const int &width, const int &height, const int &channels,
                           VkFormat textureFormat,
                           VkImageTiling tiling,
                           VkImageUsageFlags usage,
                           VkMemoryPropertyFlags memoryProperties,
                           uint32_t mipLevels,
                           VkSampleCountFlagBits msaaSamples,
                           StagingBufferMode stagingBufferMode
): VulkanResource(ctx),
   info(static_cast<uint32_t>(width), static_cast<uint32_t>(height), static_cast<uint32_t>(channels), textureFormat,
        usage),
   imageSize(width * height * channels) {
    info.tiling = tiling;
    info.setMipLevels(mipLevels).setSampleCount(msaaSamples);
    if (memoryProperties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT == 1) {
        info.isGpuOnly();
    }
    if (stagingBufferMode != StagingBufferMode::NO_STAGING_BUFFER) {
        auto stagingBufferInfo = BufferInfo::createStagingBufferInfo(imageSize,
                                                                     stagingBufferMode ==
                                                                     StagingBufferMode::PERSISTENT);
        stagingBuffer = std::make_unique<Buffer>(ctx, stagingBufferInfo);
    }
    create();
}

TextureImage::TextureImage(DeviceContext &ctx,
                           Texture2D &t2d,
                           bool generateMipMap,
                           VkSampleCountFlagBits msaaSamples,
                           StagingBufferMode stagingBufferMode)
    : TextureImage(
        ctx, t2d.getWidth(), t2d.getHeight(), 4, t2d.getFormat(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | (
            generateMipMap ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0),
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        generateMipMap ? t2d.getMaxMipLevel() : 1,
        msaaSamples,
        stagingBufferMode
    ) {
    if (stagingBufferMode != StagingBufferMode::NO_STAGING_BUFFER)
        stagingBuffer->copyToBufferMemory(t2d.pixels, 0, stagingBuffer->getSize());
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

    recorder.enqueueCommand<CopyBufferToImage>(*stagingBuffer, *this, info.dimensions.width, info.dimensions.height
    );

    recorder.recordCommandBuffer(cmd, ctx, FrameInfo::DONT_CARE);

    cmd.executeImmediate();
}

void TextureImage::transitionLayout(VkImageLayout newLayout) {
    CommandBuffer cmd{ctx, QueueFamily::QUEUE_FAMILY_GRAPHICS};
    CommandBufferRecorder recorder{};
    recorder.enqueueCommand<TransitionImageLayout>(*this,
                                                   info.format,
                                                   currentLayout,
                                                   newLayout);
    recorder.recordCommandBuffer(cmd, ctx, FrameInfo::DONT_CARE);
    cmd.executeImmediate();
    currentLayout = newLayout;
}

void TextureImage::generateMipmap(uint32_t mipLevels, VkFilter filter) {
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(ctx.get_physicalDevice(), info.format, &formatProperties);
    if (filter == VK_FILTER_LINEAR &&
        !(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw std::runtime_error("texture image format does not support linear blitting!");
    }
    CommandBuffer cmd{ctx, QueueFamily::QUEUE_FAMILY_GRAPHICS};
    CommandBufferRecorder recorder{};
    recorder.beginRecordCommandBuffer(cmd);
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = resource;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;
    int mipWidth = info.dimensions.width;
    int mipHeight = info.dimensions.height;
    for (uint32_t i = 1; i < mipLevels; i++) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        vkCmdPipelineBarrier(cmd,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);
        VkImageBlit blit{};
        blit.srcOffsets[0] = {0, 0, 0};
        blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = {0, 0, 0};
        blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;
        vkCmdBlitImage(cmd,
                       *this, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                       *this, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                       1, &blit,
                       filter);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(cmd,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }
    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(cmd,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                         0, nullptr,
                         0, nullptr,
                         1, &barrier);
    CommandBufferRecorder::endRecordCommandBuffer(cmd);
    cmd.executeImmediate();
    currentLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

TextureImage::~TextureImage() {
    vmaDestroyImage(ctx.get_vma(), resource, vmaAllocation);
    //vkDestroyImage(ctx.getLogicalDevice(), resource, nullptr);
    //vkFreeMemory(ctx.getLogicalDevice(), textureImageMemory, nullptr);
}

uint32_t TextureImage::calculateMaxMipLevels(uint32_t width, uint32_t height) {
    return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height))) + 1);
}
