//
// Created by miska on 2024/09/02.
//


#pragma once
#include <Buffer.h>
#include <Texture2D.h>

#include "ObjectHierarchy.h"

struct DeviceContext;

// bootstrapping VkImageCreateInfo wrapper, implicitly converts to VkImageCreateInfo
struct TextureImageInfo {
    int width, height;
    VkFormat format;
    VkImageUsageFlags usage;

    // flags added implicitly when setting other fields
    VkImageUsageFlags implicitUsageFlags;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
    uint32_t mipLevels = 1;
    uint32_t layers = 1;
    VkImageCreateFlags flags = 0;

    // By default, creates a simple 2D image without mip nor msaa
    TextureImageInfo(int width, int height, VkFormat format, VkImageUsageFlags usage)
        : width(width), height(height), format(format), usage(usage) {
    }
    ~TextureImageInfo() = default;
    TextureImageInfo &setSampleCount(uint32_t count);
    TextureImageInfo &setMipLevels(uint32_t count);

    operator VkImageCreateInfo() const {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
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
};

/*
 * Texture2D but on the GPU side
 */
class TextureImage : public VulkanResource<VkImage, DeviceContext>, public ObjectNode {
public:
    TextureImage(DeviceContext &ctx,
                 Texture2D &t2d,
                 bool generateMipMap = false,
                 VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT,
                 bool requiresBuffer = true);

    TextureImage(DeviceContext &ctx, const int &width, const int &height, const int &channels,
                 VkFormat textureFormat,
                 VkImageTiling tiling, VkImageUsageFlags usage,
                 VkMemoryPropertyFlags memoryProperties,
                 uint32_t mipLevels = 1,
                 VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT,
                 bool requiresBuffer = true);

    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    ~TextureImage() override;

    void stage();

    void transitionLayout(VkImageLayout newLayout);

    void generateMipmap(uint32_t mipLevels, VkFilter filter);

    [[nodiscard]] inline VkImageLayout getCurrentLayout() const {
        return currentLayout;
    };
    uint32_t getMipLevels() const { return mipLevels; };

    static uint32_t calculateMaxMipLevels(uint32_t width, uint32_t height);

private:
    VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    int width, height, channels;
    VkFormat format;
    VkDeviceSize imageSize;
    uint32_t mipLevels = 1;
    uint32_t layers = 1;
    std::unique_ptr<Buffer> stagingBuffer;
    VkDeviceMemory textureImageMemory{};
};
