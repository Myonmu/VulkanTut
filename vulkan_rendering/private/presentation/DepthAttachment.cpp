//
// Created by miska on 2024/11/10.
//

#include "DepthAttachment.h"

#include "WindowContext.h"

VkFormat DepthAttachment::findDepthFormat(std::vector<VkFormat> candidates) const {
    return ctx.get_physicalDevice().findSupportedImageFormat(
        candidates,
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

DepthAttachment::DepthAttachment(const WindowContext &ctx)
    : ctx(ctx)
      , format(findDepthFormat({
          VK_FORMAT_D32_SFLOAT,
          VK_FORMAT_D32_SFLOAT_S8_UINT,
          VK_FORMAT_D24_UNORM_S8_UINT
      })) {
    create();
}

DepthAttachment::DepthAttachment(WindowContext &ctx, VkFormat depthFormat)
    : ctx(ctx),
      format(findDepthFormat({depthFormat})) {
    create();
}

void DepthAttachment::create() {
    auto [width, height] = ctx.get_swapChain().swapChainExtent;
    depthImage = std::make_unique<TextureImage>(
        ctx.context, width,
        height, 1,
        format, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1, false);
    depthImageView = std::make_unique<ImageView>(ctx.context, *depthImage, format, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkAttachmentDescription DepthAttachment::getAttachmentDescription() const {
    VkAttachmentDescription depthAttachment{};

    depthAttachment.format = format;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    return depthAttachment;
}


VkFormat DepthAttachment::get_format() const {
    return format;
}

void DepthAttachment::recreate() {
    depthImageView.reset();
    depthImage.reset();
    create();
}

