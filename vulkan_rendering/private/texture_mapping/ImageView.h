//
// Created by miska on 10/10/2024.
//

#pragma once

#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct DeviceContext;

class ImageView : public VulkanResource<VkImageView, DeviceContext>{
public:
    ImageView(DeviceContext& ctx, const VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags);
    ~ImageView() override;
};


