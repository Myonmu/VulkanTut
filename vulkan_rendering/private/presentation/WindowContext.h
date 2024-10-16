//
// Created by miska on 2024/10/13.
//

#ifndef WINDOWCONTEXT_H
#define WINDOWCONTEXT_H
#include <FrameBuffers.h>
#include <GlfwWindow.h>
#include <QueueFamilyIndices.h>
#include <SwapChain.h>
#include <VulkanInstance.h>
#include <VulkanSurface.h>

#include "ContextMacros.h"
#include "SubContext.h"


struct DeviceContext;

struct WindowContext : SubContext<DeviceContext>{
    const char *name;
    int width;
    int height;
    QueueFamily requiredQueueFamilies;
    CTX_PROPERTY(GlfwWindow, window)
    CTX_PROPERTY(VulkanSurface, surface)
    CTX_PROPERTY(SwapChain, swapChain)
    CTX_PROPERTY(FrameBuffers, frameBuffers)

    CTX_FORWARD_GET_DECL(VulkanInstance, vulkanInstance)
    CTX_FORWARD_GET_DECL(PhysicalDevice, physicalDevice)

    static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
    ~WindowContext() override;

    WindowContext(const DeviceContext &ctx, const char *name, int width, int height, QueueFamily requiredQueueFamilies);

    // should be called after device creation
    void init();
};



#endif //WINDOWCONTEXT_H
