//
// Created by miska on 2024/09/02.
//

#ifndef TEXTUREIMAGE_H
#define TEXTUREIMAGE_H
#include <Buffer.h>
#include <Texture2D.h>

/*
 * Texture2D but on the GPU side
 */
class TextureImage: public VulkanResource<VkImage> {
public:
    TextureImage(VulkanAppContext& ctx, Texture2D& t2d);

    TextureImage(VulkanAppContext &ctx, const int &width, const int &height, const int &channels,
                 VkFormat textureFormat,
                 VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties);

    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    ~TextureImage() override;

    void stage();
    void transitionLayout(VkImageLayout newLayout);
private:
    VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    int width, height, channels;
    VkFormat format;
    VkDeviceSize imageSize;
    Buffer stagingBuffer;
    VkDeviceMemory textureImageMemory{};
};



#endif //TEXTUREIMAGE_H
