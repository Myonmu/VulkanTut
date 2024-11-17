//
// Created by miska on 2024/08/06.
//

#include "VulkanFrame.h"
#include "CommandBuffer.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <DescriptorSetLayout.h>

#include "DeviceContext.h"
#include "FrameInfo.h"
#include "RenderingContext.h"
#include <fmt/core.h>

PerFrameDescriptorAllocator::PerFrameDescriptorAllocator(WindowContext &ctx): PerFrameDescriptorAllocator(
    ctx, EngineDescriptorDef::getSet(PER_FRAME_SET_ID)) {
}

PerFrameDescriptorAllocator::PerFrameDescriptorAllocator(WindowContext &ctx,
                                                         EngineDescriptorSetCreateInfo createInfo)
    : createInfo(createInfo) {
    layout = std::make_unique<DescriptorSetLayout>(ctx.context, PER_FRAME_SET_ID, createInfo.bindings);
    allocator = std::make_unique<DescriptorAllocator>(ctx.context);
    allocator->init(2, createInfo.poolSizes);
}

std::unique_ptr<DescriptorSet> PerFrameDescriptorAllocator::allocate() const {
    return allocator->allocate(*layout);
}

PerFrameDescriptorSet::PerFrameDescriptorSet(WindowContext &ctx,
                                             PerFrameDescriptorAllocator &allocator): ctx(ctx) {
    writer = std::make_unique<DescriptorWriter>();
    perFrameSet = allocator.allocate();
    for (auto &[id, binding]: allocator.createInfo.bindings) {
        //TODO: other types of buffer?
        if (binding.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
            buffers.emplace(id, std::make_unique<Buffer>(
                                ctx.context, allocator.createInfo.bufferSizes[id],
                                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                VMA_MEMORY_USAGE_CPU_TO_GPU)
            );
        }
    }
}

void PerFrameDescriptorSet::writeBuffer(uint32_t binding, void *data, uint32_t size) {
    buffers[binding]->copyToBufferMemory(data, 0, size);
    writer->writeBuffer(binding, *buffers[binding], size, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
}

void PerFrameDescriptorSet::updateSet() const {
    writer->updateSet(ctx.getLogicalDevice(), *perFrameSet);
}

VulkanFrame::VulkanFrame(WindowContext &context, VulkanRenderer &renderer)
    : context(context),
      commandBuffer(context.context, QueueFamily::QUEUE_FAMILY_GRAPHICS) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    //Initially signaled so that we don't wait for the fence on first frame

    if (
        vkCreateSemaphore(context.getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) !=
        VK_SUCCESS ||
        vkCreateSemaphore(context.getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) !=
        VK_SUCCESS ||
        vkCreateFence(context.getLogicalDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }

    perFrameDescriptorSet = std::make_unique<PerFrameDescriptorSet>(context, *renderer.perFrameAllocator);
}

void VulkanFrame::signalResize() {
    frameBufferResized = true;
}

//TODO: Could this be modular?
void VulkanFrame::drawFrame(uint32_t currentFrameIndex, RenderingContext &renderingCtx) {
    const auto &device = context.getLogicalDevice();
    const auto &swapChain = context.get_swapChain();
    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);

    // Acquire image from swap chain
    auto result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                                        &imageIndex);

    frameInfo.imageIndex = imageIndex;
    frameInfo.windowId = context.get_windowId();
    frameInfo.currentFrameIndex = currentFrameIndex;

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        context.resize();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    //updateUniformBuffer(currentFrameIndex);
    vkResetFences(device, 1, &inFlightFence);
    // record command buffer
    vkResetCommandBuffer(commandBuffer, 0);

    renderingCtx.prepareFrame(frameInfo);

    //fmt::println("   - - - Start recording main command buffer");
    context.get_renderer().recordCommandBuffer(commandBuffer, context.context, frameInfo);
    //fmt::println("   - - - Finish recording main command buffer");
    // Submit command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffer;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (const auto &queue = context.context.getCommonQueueContext(QueueFamily::QUEUE_FAMILY_GRAPHICS).get_queue();
        vkQueueSubmit(queue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    auto &presentQueue = context.context.getPresentQueueContext(context.get_surface()).get_queue();
    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferResized) {
        frameBufferResized = false;
        context.resize();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }
}


VulkanFrame::~VulkanFrame() {
    const auto &device = context.getLogicalDevice();
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroyFence(device, inFlightFence, nullptr);
}
