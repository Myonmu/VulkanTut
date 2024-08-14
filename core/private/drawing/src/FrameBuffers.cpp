//
// Created by miska on 2024/08/04.
//

#include "FrameBuffers.h"

#include <VulkanAppContext.h>
#include <SwapChain.h>

FrameBuffers::FrameBuffers(VulkanAppContext &context) {
    this->context = &context;
    resource.resize(context.swapChain->getSize());
    for (size_t i = 0; i < context.swapChain->getSize(); i++) {
        VkImageView attachments[] = {
            context.swapChain->swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = context.renderPass->getRaw();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = context.swapChain->swapChainExtent.width;
        framebufferInfo.height = context.swapChain->swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(context.logicalDevice->getRaw(), &framebufferInfo,
            nullptr, &resource[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

FrameBuffers::~FrameBuffers() {
    for (auto framebuffer : resource) {
        vkDestroyFramebuffer(context->logicalDevice->getRaw(), framebuffer, nullptr);
    }

}

