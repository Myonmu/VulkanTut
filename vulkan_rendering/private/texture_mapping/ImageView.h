//
// Created by miska on 10/10/2024.
//

#pragma once

#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class TextureImage;
struct DeviceContext;

class ImageView : public VulkanResource<VkImageView, DeviceContext>{
public:
    ImageView(DeviceContext& ctx, const VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags);
    ImageView(DeviceContext& ctx, const TextureImage &image, VkFormat format, VkImageAspectFlags aspectFlags);
    ~ImageView() override;
};


