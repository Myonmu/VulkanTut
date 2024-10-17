//
// Created by miska on 2024/10/13.
//

#include "WindowContext.h"
#include <VulkanAppContext.h>

#include "DeviceContext.h"

WindowContext::~WindowContext() = default;

WindowContext::WindowContext(const DeviceContext &ctx, const char *name, int width, int height, QueueFamily requiredQueueFamilies)
    : SubContext(ctx), name(name), width(width), height(height), requiredQueueFamilies(requiredQueueFamilies) {
    window = std::make_unique<GlfwWindow>(ctx.context, height, width, name, &frameBufferResizeCallback);
    surface = std::make_unique<VulkanSurface>(*this);
    if (ctx.isLogicalDeviceCreated()) {
        init();
    }
}

void WindowContext::init() {
    swapChain = std::make_unique<SwapChain>(*this);
}

void WindowContext::createFrameBuffers(const RenderPass& renderPass) {

}



void WindowContext::frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
}

CTX_FORWARD_GET_BODY(WindowContext, VulkanInstance, vulkanInstance)
CTX_FORWARD_GET_BODY(WindowContext, PhysicalDevice, physicalDevice)
