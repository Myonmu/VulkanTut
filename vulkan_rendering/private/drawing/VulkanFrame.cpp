//
// Created by miska on 2024/08/06.
//

#include "VulkanFrame.h"

#include <chrono>
#include <UniformBufferObject.h>

#include "CommandBuffer.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "DeviceContext.h"
#include "FrameInfo.h"
#include "RenderingContext.h"
#include "glm/gtx/quaternion.hpp"

VulkanFrame::VulkanFrame(WindowContext &context): context(context),
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
}

void VulkanFrame::signalResize() {
    frameBufferResized = true;
}

//TODO: Could this be modular?
void VulkanFrame::drawFrame(uint32_t currentFrameIndex, RenderingContext& renderingCtx) {

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

    context.get_renderer().recordCommandBuffer(commandBuffer, context.context, frameInfo);
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

    if (const auto& queue = context.context.getCommonQueueContext(QueueFamily::QUEUE_FAMILY_GRAPHICS).get_queue();
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

    auto& presentQueue = context.context.getPresentQueueContext(context.get_surface()).get_queue();
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
