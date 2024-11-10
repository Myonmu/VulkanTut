//
// Created by miska on 2024/11/10.
//

#include "ColorAttachment.h"

#include "WindowContext.h"
#include "LogicalDevice.h"

void ColorAttachment::create() {
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

ColorAttachment::ColorAttachment(const WindowContext &ctx): ctx(ctx) {
    create();
}

size_t ColorAttachment::getSize() const {
    return swapChainImageViews.size();
}

ImageView &ColorAttachment::get_imageView_at(const uint32_t i) const {
    return *swapChainImageViews.at(i);
}

VkFormat ColorAttachment::get_format() const {
    return format;
}


VkAttachmentDescription ColorAttachment::getAttachmentDescription() const {
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

void ColorAttachment::recreate() {
    swapChainImageViews.clear();
    swapChainImages.clear();
    create();
}
