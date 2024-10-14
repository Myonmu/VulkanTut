//
// Created by miska on 2024/08/06.
//

#include <VulkanAppContext.h>

VulkanAppContext::VulkanAppContext(int w, int h, const char *appName)
    : name(appName),
      width(w),
      height(h)
       {
}

VulkanAppContext::~VulkanAppContext() = default;

void VulkanAppContext::resize() {
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(logicalDevice);
    swapChain.recreate();
    frameBuffers.recreate();
}

void VulkanAppContext::drawFrame() {
    renderer.drawFrame();
}


void VulkanAppContext::frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
    const auto app = static_cast<VulkanAppContext *>(glfwGetWindowUserPointer(window));
    app->renderer.signalResize();
}

const LogicalDevice& VulkanAppContext::getLogicalDevice() const {
    return logicalDevice;
}
