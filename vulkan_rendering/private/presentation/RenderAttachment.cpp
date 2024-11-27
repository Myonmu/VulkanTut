//
// Created by miska on 2024/11/10.
//

#include "RenderAttachment.h"
#include <TextureImage.h>
#include "WindowContext.h"
#include "LogicalDevice.h"


AttachmentRef::AttachmentRef(uint32_t id, const RenderAttachment &attachment, VkImageLayout layout)
    : index(id), type(attachment.getAttachmentType())
      , description(attachment.getAttachmentDescription()), layout(layout) {
}


void ColorAttachment::create() {
    auto &swapChain = ctx.get_swapChain();
    format = swapChain.swapChainImageFormat;
    image = std::make_unique<TextureImage>(
        ctx.context,
        swapChain.swapChainExtent.width, swapChain.swapChainExtent.height,
        4, format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        1, msaaSamples, false
    );
    imageView = std::make_unique<ImageView>(ctx.context, *image, format, VK_IMAGE_ASPECT_COLOR_BIT);
}

ColorAttachment::ColorAttachment(const WindowContext &ctx, VkSampleCountFlagBits msaaSamples, AttachmentType type)
    : ctx(ctx), type(type), msaaSamples(msaaSamples) {
    create();
}

void ColorAttachment::recreate() {
    image.reset();
    imageView.reset();
    create();
}

VkAttachmentDescription ColorAttachment::getAttachmentDescription() const {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = msaaSamples;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    return colorAttachment;
}


void PresentColorAttachment::create() {
    auto &swapChain = ctx.get_swapChain();
    format = swapChain.swapChainImageFormat;
    uint32_t count;
    vkGetSwapchainImagesKHR(ctx.getLogicalDevice(), swapChain, &count, nullptr);
    swapChainImages.resize(count);
    vkGetSwapchainImagesKHR(ctx.getLogicalDevice(), swapChain, &count, swapChainImages.data());

    for (auto &swapChainImage: swapChainImages) {
        swapChainImageViews.emplace_back(
            std::make_unique<ImageView>(ctx.context, swapChainImage, format, VK_IMAGE_ASPECT_COLOR_BIT)
        );
    }
}

PresentColorAttachment::PresentColorAttachment(const WindowContext &ctx): ctx(ctx) {
    create();
}

size_t PresentColorAttachment::getSize() const {
    return swapChainImageViews.size();
}

ImageView &PresentColorAttachment::get_imageView_at(const uint32_t i) const {
    return *swapChainImageViews.at(i);
}

VkFormat PresentColorAttachment::get_format() const {
    return format;
}


VkAttachmentDescription PresentColorAttachment::getAttachmentDescription() const {
    VkAttachmentDescription colorAttachment{};
    // we *may* use swap chain image format but not necessarily.
    // VkRenderPass can be shared between swap chains
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    return colorAttachment;
}

void PresentColorAttachment::recreate() {
    swapChainImageViews.clear();
    swapChainImages.clear();
    create();
}


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
      })), msaaSamples(VK_SAMPLE_COUNT_1_BIT) {
    create();
}

DepthAttachment::DepthAttachment(WindowContext &ctx, VkFormat depthFormat, VkSampleCountFlagBits msaaSamples)
    : ctx(ctx),
      format(findDepthFormat({depthFormat})),
      msaaSamples(ctx.get_physicalDevice().getClampedMsaaSampleCount(msaaSamples)) {
    create();
}

void DepthAttachment::create() {
    auto [width, height] = ctx.get_swapChain().swapChainExtent;
    depthImage = std::make_unique<TextureImage>(
        ctx.context, width,
        height, 1,
        format, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1,
        msaaSamples,
        false);
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (format >= VK_FORMAT_D16_UNORM_S8_UINT) aspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
    depthImageView = std::make_unique<ImageView>(ctx.context, *depthImage, format, aspect);
}

VkAttachmentDescription DepthAttachment::getAttachmentDescription() const {
    VkAttachmentDescription depthAttachment{};

    depthAttachment.format = format;
    depthAttachment.samples = msaaSamples;
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


ImageView &AttachmentManager::getOrCreateAttachment(VkAttachmentDescription description) {

}
