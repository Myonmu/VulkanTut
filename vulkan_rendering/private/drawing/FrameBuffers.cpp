//
// Created by miska on 2024/08/04.
//

#include "FrameBuffers.h"

#include <VulkanAppContext.h>
#include <SwapChain.h>

#include "WindowContext.h"

void FrameBuffers::createFrameBuffers() {
    resource.resize(ctx.swapChain.getSize());
    for (size_t i = 0; i < ctx.swapChain.getSize(); i++) {
        VkImageView attachments[] = {
            ctx.swapChain.swapChainImageViews[i]->getRaw()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = ctx.renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = ctx.swapChain.swapChainExtent.width;
        framebufferInfo.height = ctx.swapChain.swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(ctx.context.logicalDevice, &framebufferInfo,
            nullptr, &resource[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void FrameBuffers::destroyFrameBuffers() {
    for (auto framebuffer : resource) {
        vkDestroyFramebuffer(ctx.getLogicalDevice(), framebuffer, nullptr);
    }

}


FrameBuffers::FrameBuffers(WindowContext &context): VulkanResource(context) {
    createFrameBuffers();
}

FrameBuffers::~FrameBuffers() {
    destroyFrameBuffers();
}

void FrameBuffers::recreate() {
    destroyFrameBuffers();
    createFrameBuffers();
}


