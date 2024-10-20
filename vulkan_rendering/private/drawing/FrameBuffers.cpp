//
// Created by miska on 2024/08/04.
//

#include "FrameBuffers.h"

#include <VulkanAppContext.h>
#include <SwapChain.h>

#include "WindowContext.h"
#include "LogicalDevice.h"

void FrameBuffers::createFrameBuffers(const RenderPass& renderPass) {
    auto& swapChain = ctx.get_swapChain();
    const auto swapChainSize = swapChain.getSize();
    resource.resize(swapChainSize);
    for (size_t i = 0; i < swapChainSize; i++) {
        VkImageView attachments[] = {
            swapChain.swapChainImageViews[i]->getRaw()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain.swapChainExtent.width;
        framebufferInfo.height = swapChain.swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(ctx.getLogicalDevice(), &framebufferInfo,
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


FrameBuffers::FrameBuffers(WindowContext &context, const RenderPass& renderPass): VulkanResource(context)
, renderPass(renderPass){
    createFrameBuffers(renderPass);
}

FrameBuffers::~FrameBuffers() {
    destroyFrameBuffers();
}

void FrameBuffers::recreate() {
    destroyFrameBuffers();
    createFrameBuffers(renderPass);
}


