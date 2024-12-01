//
// Created by miska on 10/10/2024.
//

#include "ImageView.h"

#include <VulkanAppContext.h>

#include "DeviceContext.h"

ImageViewInfo::ImageViewInfo(const TextureImageInfo &textureImageInfo, ImageViewType viewType) {
    type = viewType;
    format = textureImageInfo.format;
    baseMipLevel = 0;
    levelCount = textureImageInfo.mipLevels;
    baseArrayLayer = 0;
    layerCount = textureImageInfo.layers;
    if (textureImageInfo.usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
        aspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
        if (format >= VK_FORMAT_D16_UNORM_S8_UINT) aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    else if (textureImageInfo.usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
        aspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
    }
    else if (textureImageInfo.usage & VK_IMAGE_USAGE_SAMPLED_BIT) { //TODO: always true?
        aspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
    }
}


ImageView::ImageView(DeviceContext &ctx, ImageViewInfo &info, TextureImage &image): VulkanResource(ctx), info(info) {
    VkImageViewCreateInfo viewInfo = info;
    viewInfo.image = image;
    if (vkCreateImageView(ctx.getLogicalDevice(), &viewInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

ImageView::ImageView(DeviceContext &ctx, ImageViewInfo &info, VkImage &image): VulkanResource(ctx), info(info) {
    VkImageViewCreateInfo viewInfo = info;
    viewInfo.image = image;
    if (vkCreateImageView(ctx.getLogicalDevice(), &viewInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

ImageView::~ImageView() {
    vkDestroyImageView(ctx.getLogicalDevice(), resource, nullptr);
}
