//
// Created by miska on 2024/10/13.
//

#ifndef WINDOWCONTEXT_H
#define WINDOWCONTEXT_H
#include <FrameBuffers.h>
#include <GlfwWindow.h>
#include <SwapChain.h>
#include <VulkanSurface.h>

#include "SubContext.h"


struct WindowContext : SubContext<VulkanAppContext>{
    const char *name;
    int width;
    int height;
    GlfwWindow window{*this, width, height, name, frameBufferResizeCallback};
    VulkanSurface surface;
    SwapChain swapChain{*this};
    FrameBuffers frameBuffers{*this};

    static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
    ~WindowContext() override;

    WindowContext(const VulkanAppContext &ctx, const char *name, int width, int height);
};



#endif //WINDOWCONTEXT_H
