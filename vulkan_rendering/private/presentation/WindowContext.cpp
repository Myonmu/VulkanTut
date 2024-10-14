//
// Created by miska on 2024/10/13.
//

#include "WindowContext.h"
#include <VulkanAppContext.h>

WindowContext::~WindowContext() = default;

WindowContext::WindowContext(const VulkanAppContext &ctx, const char *name,
                             int width,
                             int height )
    : SubContext(ctx), name(name), width(width), height(height)
{
}

void WindowContext::frameBufferResizeCallback(GLFWwindow *window, int width, int height) {

}

