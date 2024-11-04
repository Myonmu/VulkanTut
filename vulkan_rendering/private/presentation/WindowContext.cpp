//
// Created by miska on 2024/10/13.
//

#include "WindowContext.h"
#include <VulkanAppContext.h>
#include "GlfwWindow.h"
#include "DeviceContext.h"

WindowContext::~WindowContext() = default;

WindowContext::WindowContext(DeviceContext &ctx, const char *name, int width, int height, QueueFamily requiredQueueFamilies)
    : SubContext(ctx), name(name), width(width), height(height), requiredQueueFamilies(requiredQueueFamilies) {
    window = std::make_unique<GlfwWindow>(*this, width, height, name, &frameBufferResizeCallback);
    surface = std::make_unique<VulkanSurface>(*this);
    if (ctx.isLogicalDeviceCreated()) {
        init();
    }
}

void WindowContext::init() {
    swapChain = std::make_unique<SwapChain>(*this);
}

void WindowContext::createFrameBuffers(const RenderPass& renderPass) {
    if (frameBuffers != nullptr) return;
    frameBuffers = std::make_unique<FrameBuffers>(*this, renderPass);
    renderer = std::make_unique<VulkanRenderer>(*this);
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
    auto* w = static_cast<WindowContext *>(glfwGetWindowUserPointer(window));
    w->resize();
}

void WindowContext::closeWindow() {
    std::cout << "Closing window" << id << std::endl;
    context.destroyWindow(id);
}

LogicalDevice &WindowContext::getLogicalDevice() const {
    return context.get_logicalDevice();
}


CTX_FORWARD_GET_BODY(WindowContext, VulkanInstance, vulkanInstance)
CTX_FORWARD_GET_BODY(WindowContext, PhysicalDevice, physicalDevice)
