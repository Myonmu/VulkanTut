//
// Created by miska on 2024/09/02.
//


#pragma once
#include <Buffer.h>
#include <Texture2D.h>

#include "ObjectHierarchy.h"

struct DeviceContext;
/*
 * Texture2D but on the GPU side
 */
class TextureImage: public VulkanResource<VkImage, DeviceContext>, public ObjectNode {
public:
    TextureImage(DeviceContext& ctx, Texture2D& t2d, bool generateMipMap = false, bool requiresBuffer = true);

    TextureImage(DeviceContext &ctx, const int &width, const int &height, const int &channels,
                 VkFormat textureFormat,
                 VkImageTiling tiling, VkImageUsageFlags usage,
                 VkMemoryPropertyFlags memoryProperties,
                 uint32_t mipLevels = 1,
                 bool requiresBuffer = true);

    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    ~TextureImage() override;

    void stage();
    void transitionLayout(VkImageLayout newLayout);
    void generateMipmap(uint32_t mipLevels, VkFilter filter);
    [[nodiscard]] inline VkImageLayout getCurrentLayout() const {
        return currentLayout;
    };
    uint32_t getMipLevels() const {return mipLevels;};

    static uint32_t calculateMaxMipLevels(uint32_t width, uint32_t height);
private:
    VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    int width, height, channels;
    VkFormat format;
    VkDeviceSize imageSize;
    uint32_t mipLevels = 1;
    std::unique_ptr<Buffer> stagingBuffer;
    VkDeviceMemory textureImageMemory{};
};



