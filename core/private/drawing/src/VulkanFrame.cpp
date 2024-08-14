//
// Created by miska on 2024/08/06.
//

#include "VulkanFrame.h"

#include <VulkanAppContext.h>

#include "CommandBuffer.h"

VulkanFrame::VulkanFrame(VulkanAppContext &context) {
    commandBuffer = new CommandBuffer(context);
    this->context = &context;
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    //Initially signaled so that we don't wait for the fence on first frame

    if (
        vkCreateSemaphore(context.logicalDevice->getRaw(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) !=
        VK_SUCCESS ||
        vkCreateSemaphore(context.logicalDevice->getRaw(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) !=
        VK_SUCCESS ||
        vkCreateFence(context.logicalDevice->getRaw(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }
}

void VulkanFrame::signalResize() {
    frameBufferResized = true;
}


void VulkanFrame::drawFrame() {
    auto device = context->logicalDevice->getRaw();
    auto swapChain = context->swapChain->getRaw();
    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);

    // Acquire image from swap chain
    auto result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                                        &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        context->resize();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to aquire swap chain image");
    }

    vkResetFences(device, 1, &inFlightFence);
    // record command buffer
    vkResetCommandBuffer(commandBuffer->getRaw(), 0);
    commandBufferRecorder.recordCommandBuffer(context, commandBuffer, imageIndex);
    // Submit command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffer->getRawPtr();

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(context->logicalDevice->graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
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

    result = vkQueuePresentKHR(context->logicalDevice->presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferResized) {
        frameBufferResized = false;
        context->resize();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }
}

VulkanFrame::~VulkanFrame() {
    const auto device = context->logicalDevice->getRaw();
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroyFence(device, inFlightFence, nullptr);
}
