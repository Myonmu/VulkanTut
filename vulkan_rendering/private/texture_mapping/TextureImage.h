//
// Created by miska on 2024/09/02.
//


#pragma once
#include <Buffer.h>
#include <Texture2D.h>

struct DeviceContext;
/*
 * Texture2D but on the GPU side
 */
class TextureImage: public VulkanResource<VkImage, DeviceContext> {
public:
    TextureImage(DeviceContext& ctx, Texture2D& t2d);

    TextureImage(DeviceContext &ctx, const int &width, const int &height, const int &channels,
                 VkFormat textureFormat,
                 VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties);

    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    ~TextureImage() override;

    void stage();
    void transitionLayout(VkImageLayout newLayout);
    [[nodiscard]] inline VkImageLayout getCurrentLayout() const {
        return currentLayout;
    };
private:
    VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    int width, height, channels;
    VkFormat format;
    VkDeviceSize imageSize;
    Buffer stagingBuffer;
    VkDeviceMemory textureImageMemory{};
};



