//
// Created by miska on 2024/02/10.
//

#include <stdexcept>
#include "VulkanSurface.h"
#include "VulkanAppContext.h"

void VulkanSurface::createSurface(VulkanAppContext& context){
    if(glfwCreateWindowSurface(context.vulkanInstance,
                               context.window, nullptr, &resource) != VK_SUCCESS){
        throw std::runtime_error("failed to create window surface");
    }
}

VulkanSurface::VulkanSurface(VulkanAppContext &context): VulkanResource(context) {
    createSurface(context);
}

VulkanSurface::~VulkanSurface() {
    vkDestroySurfaceKHR(ctx.vulkanInstance, resource, nullptr);
}
