//
// Created by miska on 2024/11/10.
//

#pragma once
#include <TextureImage.h>
#include <vulkan/vulkan_core.h>
#include "ImageView.h"


struct WindowContext;

class DepthAttachment {
    const WindowContext &ctx;

    VkFormat format;
    std::unique_ptr<TextureImage> depthImage;
    std::unique_ptr<ImageView> depthImageView;

    VkFormat findDepthFormat(std::vector<VkFormat> candidates) const;

    static bool hasStencilComponent(const VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void create();

public:
    explicit DepthAttachment(const WindowContext &ctx);

    DepthAttachment(WindowContext &ctx, VkFormat depthFormat);

    [[nodiscard]] VkAttachmentDescription getAttachmentDescription() const;

    [[nodiscard]] VkFormat get_format() const;

    void recreate();

    operator VkImageView() const { return *depthImageView; }
};
