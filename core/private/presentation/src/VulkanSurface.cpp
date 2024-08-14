//
// Created by miska on 2024/02/10.
//

#include <stdexcept>
#include "VulkanSurface.h"
#include "VulkanAppContext.h"

void VulkanSurface::createSurface(VulkanAppContext& context){
    if(glfwCreateWindowSurface(context.vulkanInstance->getRaw(),
                               context.window, nullptr, &resource) != VK_SUCCESS){
        throw std::runtime_error("failed to create window surface");
    }
}

VulkanSurface::VulkanSurface(VulkanAppContext &context) : VulkanResource<VkSurfaceKHR>(){
    ctx = &context;
    createSurface(context);
}

VulkanSurface::~VulkanSurface() {
    vkDestroySurfaceKHR(ctx->vulkanInstance->getRaw(), resource, nullptr);
}
