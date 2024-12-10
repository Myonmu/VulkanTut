//
// Created by miska on 10/10/2024.
//

#pragma once

#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct TextureImageInfo;
class TextureImage;
struct DeviceContext;

enum class ImageViewType {
    SIMPLE_1D = VK_IMAGE_VIEW_TYPE_1D,
    SIMPLE_2D = VK_IMAGE_VIEW_TYPE_2D,
    SIMPLE_3D = VK_IMAGE_VIEW_TYPE_3D,
    CUBE = VK_IMAGE_VIEW_TYPE_CUBE,
    ARRAY_1D = VK_IMAGE_VIEW_TYPE_1D_ARRAY,
    ARRAY_2D = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
    ARRAY_CUBE = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
};

struct ImageViewInfo {
    VkFormat format{};
    ImageViewType type = ImageViewType::SIMPLE_2D;
    VkImageAspectFlags aspectFlags = 0;
    uint32_t baseMipLevel = 0;
    uint32_t levelCount = 1;
    uint32_t baseArrayLayer = 0;
    uint32_t layerCount = 1;

    explicit ImageViewInfo(const VkFormat format): format(format) {}
    explicit ImageViewInfo(const TextureImageInfo& textureImageInfo, ImageViewType viewType = ImageViewType::SIMPLE_2D);

    operator VkImageViewCreateInfo() const {
        VkImageViewCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.viewType = static_cast<VkImageViewType>(type);
        info.format = format;
        info.subresourceRange.baseMipLevel = baseMipLevel;
        info.subresourceRange.levelCount = levelCount;
        info.subresourceRange.baseArrayLayer = baseArrayLayer;
        info.subresourceRange.layerCount = layerCount;
        info.subresourceRange.aspectMask = aspectFlags;
        return info;
    }
};

class ImageView : public VulkanResource<VkImageView, DeviceContext>{
    ImageViewInfo info;
public:
    ImageView(DeviceContext& ctx, ImageViewInfo& info, TextureImage &image);
    ImageView(DeviceContext& ctx, ImageViewInfo& info, VkImage &image);
    ~ImageView() override;
};


