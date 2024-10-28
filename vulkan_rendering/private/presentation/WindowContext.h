//
// Created by miska on 2024/10/13.
//

#pragma once

#include <FrameBuffers.h>
#include <GlfwWindow.h>
#include <QueueFamilyIndices.h>
#include <SwapChain.h>
#include <VulkanInstance.h>
#include <VulkanRenderer.h>
#include <VulkanSurface.h>

#include "ContextMacros.h"
#include "SubContext.h"


class RenderPass;
struct DeviceContext;

struct WindowContext : public SubContext<DeviceContext>{
    friend struct DeviceContext;
    const char *name;
    int width;
    int height;
    QueueFamily requiredQueueFamilies;
    CTX_PROPERTY(GlfwWindow, window)

    CTX_PROPERTY(VulkanSurface, surface)
    // requires device
    CTX_PROPERTY(SwapChain, swapChain)
    // requires render pass
    CTX_PROPERTY(FrameBuffers, frameBuffers)

    CTX_PROPERTY(VulkanRenderer, renderer)

    CTX_FORWARD_GET_DECL(VulkanInstance, vulkanInstance)
    CTX_FORWARD_GET_DECL(PhysicalDevice, physicalDevice)

    static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
    ~WindowContext() override;

    WindowContext(DeviceContext &ctx, const char *name, int width, int height, QueueFamily requiredQueueFamilies);

    void resize();

    void closeWindow();

    void createFrameBuffers(const RenderPass& renderPass);

    [[nodiscard]] uint32_t get_windowId() const{ return id;}
private:
    uint32_t id;
    // should be called after device creation
    void init();
};

