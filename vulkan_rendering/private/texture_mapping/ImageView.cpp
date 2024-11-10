//
// Created by miska on 10/10/2024.
//

#include "ImageView.h"

#include <VulkanAppContext.h>

#include "DeviceContext.h"

ImageView::ImageView(DeviceContext &ctx, const VkImage &image, const VkFormat format, const VkImageAspectFlags aspectFlags)
: VulkanResource(ctx) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(ctx.getLogicalDevice(), &viewInfo, nullptr, &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

ImageView::~ImageView() {
    vkDestroyImageView(ctx.getLogicalDevice(), resource, nullptr);
}
