//
// Created by miska on 2024/11/10.
//

#include "RenderAttachment.h"
#include <TextureImage.h>
#include "WindowContext.h"
#include "LogicalDevice.h"

bool RenderAttachment::isCompatibleWith(AttachmentInfo &ref) {
    throw std::runtime_error("RenderAttachment::isCompatibleWith not implemented");
}


// vvvvvvvvvvvvvvvvvvvvvvvvvvvv Color Attachment

void ColorAttachment::create() {
    auto &swapChain = ctx.get_swapChain();
    format = swapChain.swapChainImageFormat;
    TextureImageInfo info{
        swapChain.swapChainExtent.width, swapChain.swapChainExtent.height, 4, format,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    };

    info.setSampleCount(msaaSamples).isGpuOnly().isInputAttachment().isTransientAttachment(); //TODO: not always

    image = std::make_unique<TextureImage>(
        ctx.context, info, StagingBufferMode::NO_STAGING_BUFFER
    );

    ImageViewInfo viewInfo{info};
    imageView = std::make_unique<ImageView>(ctx.context, viewInfo, *image);
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

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Color Attachment

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv Present Color Attachment

void PresentColorAttachment::create() {
    auto &swapChain = ctx.get_swapChain();
    format = swapChain.swapChainImageFormat;
    uint32_t count;
    vkGetSwapchainImagesKHR(ctx.getLogicalDevice(), swapChain, &count, nullptr);
    swapChainImages.resize(count);
    vkGetSwapchainImagesKHR(ctx.getLogicalDevice(), swapChain, &count, swapChainImages.data());

    ImageViewInfo viewInfo{format};
    viewInfo.aspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT;

    for (auto &swapChainImage: swapChainImages) {
        swapChainImageViews.emplace_back(
            std::make_unique<ImageView>(ctx.context, viewInfo, swapChainImage)
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

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Present Color Attachment

// vvvvvvvvvvvvvvvvvvvvvvvvvvvv Depth Attachment

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
    TextureImageInfo info{
        width, height, 1, format, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
    };
    info.setSampleCount(msaaSamples).isGpuOnly();
    depthImage = std::make_unique<TextureImage>(ctx.context, info);

    ImageViewInfo viewInfo{info};
    depthImageView = std::make_unique<ImageView>(ctx.context, viewInfo, *depthImage);
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


// ^^^^^^^^^^^^^^^^^^^^^^^^^^  Depth Attachment


AttachmentManager::AttachmentManager(WindowContext &ctx): ctx(ctx) {

}


RenderAttachment &AttachmentManager::getOrCreateAttachment(const AttachmentInfo &attachmentRef) {
    /*
    switch (attachmentRef.type) {
        case AttachmentType::DEPTH_STENCIL:
            return REFERENCE_CAST(attachments.emplace_back<DepthAttachment>(ctx), RenderAttachment);
        case AttachmentType::PRESENT:
            return REFERENCE_CAST(attachments.emplace_back<PresentColorAttachment>(ctx), RenderAttachment);
            break;
        case AttachmentType::COLOR:
            return REFERENCE_CAST(attachments.emplace_back<ColorAttachment>(ctx), RenderAttachment);
            break;
    }*/
    throw std::runtime_error("Not implemented");
}
