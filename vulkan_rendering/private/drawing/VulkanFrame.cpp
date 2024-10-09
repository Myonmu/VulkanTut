//
// Created by miska on 2024/08/06.
//

#include "VulkanFrame.h"

#include <chrono>
#include <UniformBufferObject.h>
#include <VulkanAppContext.h>

#include "CommandBuffer.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "FrameInfo.h"
#include "glm/gtx/quaternion.hpp"

VulkanFrame::VulkanFrame(VulkanAppContext &context): context(context),
                                                     commandBuffer(context) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    //Initially signaled so that we don't wait for the fence on first frame

    if (
        vkCreateSemaphore(context.logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore) !=
        VK_SUCCESS ||
        vkCreateSemaphore(context.logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore) !=
        VK_SUCCESS ||
        vkCreateFence(context.logicalDevice, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }
}

void VulkanFrame::signalResize() {
    frameBufferResized = true;
}

//TODO: Could this be modular?
void VulkanFrame::drawFrame(uint32_t currentFrameIndex) {
    const auto& device = context.logicalDevice;
    const auto& swapChain = context.swapChain;
    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);

    // Acquire image from swap chain
    auto result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                                        &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        context.resize();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }
    updateUniformBuffer(currentFrameIndex);
    vkResetFences(device, 1, &inFlightFence);
    // record command buffer
    vkResetCommandBuffer(commandBuffer, 0);
    frameInfo.imageIndex = imageIndex;
    frameInfo.currentFrameIndex = currentFrameIndex;
    context.frameDrawer.recordCommandBuffer(commandBuffer, context,  frameInfo);
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

    if (vkQueueSubmit(context.logicalDevice.graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
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

    result = vkQueuePresentKHR(context.logicalDevice.presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferResized) {
        frameBufferResized = false;
        context.resize();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }
}

void VulkanFrame::updateUniformBuffer(uint32_t currentFrame) {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration<float, std::chrono::seconds::period> (currentTime - startTime).count();
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time*glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f,2,2), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.projection = glm::perspective(glm::radians(45.0f),
        context.swapChain.swapChainExtent.width/(float)context.swapChain.swapChainExtent.height, 0.1f, 10.0f);
    /*
     *GLM was originally designed for OpenGL,
     *where the Y coordinate of the clip coordinates is inverted.
     *The easiest way to compensate for that is to
     *flip the sign on the scaling factor of the Y axis in the projection matrix.
     *If you don't do this, then the image will be rendered upside down.
     */
    ubo.projection[1][1] *= -1;

    context.uniformBufferGroup.CopyMemoryToBuffer(currentFrame, &ubo, sizeof(ubo));
}


VulkanFrame::~VulkanFrame() {
    const auto& device = context.logicalDevice;
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroyFence(device, inFlightFence, nullptr);
}
