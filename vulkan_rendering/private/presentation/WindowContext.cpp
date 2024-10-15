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
}

void WindowContext::frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
}

CTX_FORWARD_GET_BODY(WindowContext, VulkanInstance, vulkanInstance)
CTX_FORWARD_GET_BODY(WindowContext, PhysicalDevice, physicalDevice)
