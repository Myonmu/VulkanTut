//
// Created by miska on 2024/09/02.
//


#pragma once
#include <Buffer.h>
#include <Texture2D.h>

#include "ObjectHierarchy.h"

struct DeviceContext;

struct TexturePxDimensions {
    uint32_t width = 0;
    uint32_t height = 0;
    // only makes sense for 3d textures.
    uint32_t depth = 1;

    TexturePxDimensions();
    TexturePxDimensions(uint32_t width, uint32_t height, uint32_t depth):width(width), height(height), depth(depth) {}
    TexturePxDimensions(uint32_t width, uint32_t height):width(width), height(height) {}

    bool operator==(const TexturePxDimensions &texture_dimensions) const = default;

    // Calculates the maximum mip levels
    [[nodiscard]] uint32_t getLevels() const{
        uint32_t levels = 0;
        uint32_t max_dim = std::max(std::max(width, height), depth);
        while (max_dim) {
            levels++;
            max_dim >>= 1;
        }
        return levels;
    };
};


// bootstrapping VkImageCreateInfo wrapper, implicitly converts to VkImageCreateInfo
struct TextureImageInfo: VmaAllocatedResourceInfo<TextureImageInfo> {
    TexturePxDimensions dimensions;
    // theoretically, we could just get channels from format, but it is tedious
    uint32_t channels;
    VkImageType type = VK_IMAGE_TYPE_2D;
    VkFormat format;
    VkImageUsageFlags usage;
    // flags added implicitly when setting other fields
    VkImageUsageFlags implicitUsageFlags = 0;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
    uint32_t mipLevels = 1;
    uint32_t layers = 1;
    VkImageCreateFlags flags = 0;

    // By default, creates a simple 2D image without mip nor msaa
    TextureImageInfo(uint32_t width, uint32_t height, uint32_t channels, VkFormat format, VkImageUsageFlags usage)
        : dimensions(width, height),channels(channels), format(format), usage(usage) {
    }

    TextureImageInfo(TexturePxDimensions &dimensions, uint32_t channels, VkFormat format, VkImageUsageFlags usage)
        :dimensions(dimensions), channels(channels), format(format), usage(usage) {
    }

    ~TextureImageInfo() override = default;

    TextureImageInfo &setSampleCount(uint32_t count);

    TextureImageInfo &setMipLevels(uint32_t count);

    TextureImageInfo &setMaxMipLevels();

    TextureImageInfo &setLayers(uint32_t count);

    TextureImageInfo &isInputAttachment();

    TextureImageInfo &isTransientAttachment();

    uint32_t getSize() const;

    inline operator VkImageCreateInfo() const;

    inline bool operator==(const TextureImageInfo &other) const;
};

/*
 * Texture2D but on the GPU side
 */
class TextureImage : public VulkanResource<VkImage, DeviceContext>, public ObjectNode {
public:
    TextureImage(DeviceContext &ctx, TextureImageInfo &info,
                 StagingBufferMode stagingBufferMode = StagingBufferMode::MAP_PER_CALL);

    TextureImage(DeviceContext &ctx,
                 Texture2D &t2d,
                 bool generateMipMap = false,
                 VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT,
                 StagingBufferMode stagingBufferMode = StagingBufferMode::MAP_PER_CALL);

    TextureImage(DeviceContext &ctx, const int &width, const int &height, const int &channels,
                 VkFormat textureFormat,
                 VkImageTiling tiling, VkImageUsageFlags usage,
                 VkMemoryPropertyFlags memoryProperties,
                 uint32_t mipLevels = 1,
                 VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT,
                 StagingBufferMode stagingBufferMode = StagingBufferMode::MAP_PER_CALL);

    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    ~TextureImage() override;

    void stage();

    void transitionLayout(VkImageLayout newLayout);

    void generateMipmap(uint32_t mipLevels, VkFilter filter);

    [[nodiscard]] VkImageLayout getCurrentLayout() const {
        return currentLayout;
    };

    [[nodiscard]] uint32_t getMipLevels() const { return info.mipLevels; };

    static uint32_t calculateMaxMipLevels(uint32_t width, uint32_t height);

    const TextureImageInfo& get_info() const {return info;}

private:
    void create();

    VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    TextureImageInfo info;
    VkDeviceSize imageSize{};
    std::unique_ptr<Buffer> stagingBuffer;
    VkDeviceMemory textureImageMemory{};
    VmaAllocation vmaAllocation{};
};
