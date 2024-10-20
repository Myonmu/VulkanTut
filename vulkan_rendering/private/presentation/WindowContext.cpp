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
    frameBuffers = std::make_unique<FrameBuffers>(*this, renderPass);
}

void WindowContext::resize() {
    glfwGetFramebufferSize(get_window(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(get_window(), &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(getLogicalDevice());
    swapChain->recreate();
    frameBuffers->recreate();
}

void WindowContext::frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
    const auto app = static_cast<VulkanAppContext *>(glfwGetWindowUserPointer(window));
    app->renderer.signalResize();
}

CTX_FORWARD_GET_BODY(WindowContext, VulkanInstance, vulkanInstance)
CTX_FORWARD_GET_BODY(WindowContext, PhysicalDevice, physicalDevice)
