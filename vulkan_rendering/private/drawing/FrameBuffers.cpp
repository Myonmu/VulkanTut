//
// Created by miska on 2024/08/04.
//

#include "FrameBuffers.h"

#include <array>
#include <VulkanAppContext.h>
#include <SwapChain.h>

#include "WindowContext.h"
#include "LogicalDevice.h"

void FrameBuffer::createFrameBuffers(const RenderPass &renderPass) {
    auto &swapChain = ctx.get_swapChain();
    auto &colorAttachment = ctx.get_colorAttachment();
    const auto swapChainSize = colorAttachment.getSize();
    resource.resize(swapChainSize);
    for (size_t i = 0; i < swapChainSize; i++) {
        auto &color = colorAttachment.get_imageView_at(i);
        auto &depth = ctx.get_depthAttachment();
        auto &pos = ctx.get_gbufferPosition();
        auto &normal = ctx.get_gbufferNormal();
        auto &albedo = ctx.get_gbufferAlbedo();
        //TODO: generate this based on renderpass (even better, signal creation of the attachments HERE)
        std::array<VkImageView, 5> attachments = {
            color, pos, normal, albedo, depth,
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChain.swapChainExtent.width;
        framebufferInfo.height = swapChain.swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(ctx.getLogicalDevice(), &framebufferInfo,
                                nullptr, &resource[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void FrameBuffer::destroyFrameBuffers() {
    for (auto framebuffer: resource) {
        vkDestroyFramebuffer(ctx.getLogicalDevice(), framebuffer, nullptr);
    }
}


FrameBuffer::FrameBuffer(WindowContext &context, const RenderPass &renderPass): VulkanResource(context)
    , renderPass(renderPass) {
    createFrameBuffers(renderPass);
}

FrameBuffer::~FrameBuffer() {
    destroyFrameBuffers();
}

void FrameBuffer::recreate() {
    destroyFrameBuffers();
    createFrameBuffers(renderPass);
}
