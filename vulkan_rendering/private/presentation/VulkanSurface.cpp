//
// Created by miska on 2024/02/10.
//

#include <stdexcept>
#include "VulkanSurface.h"

#include "DeviceContext.h"
#include "VulkanAppContext.h"
#include "WindowContext.h"

void VulkanSurface::createSurface(WindowContext& context){
    if(glfwCreateWindowSurface(context.context.context.get_vulkanInstance(),
                               context.get_window(), nullptr, &resource) != VK_SUCCESS){
        throw std::runtime_error("failed to create window surface");
    }
}

VulkanSurface::VulkanSurface(WindowContext &context): VulkanResource(context) {
    createSurface(context);
}

VulkanSurface::~VulkanSurface() {
    vkDestroySurfaceKHR(ctx.context.context.get_vulkanInstance(), resource, nullptr);
}
