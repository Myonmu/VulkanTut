//
// Created by miska on 2024/02/10.
//

#include <stdexcept>
#include <SDL3/SDL_vulkan.h>
#include "VulkanSurface.h"
#include "DeviceContext.h"
#include "WindowContext.h"

void VulkanSurface::createSurface(WindowContext& context){
    auto result = SDL_Vulkan_CreateSurface(context.get_window(),
        context.get_vulkanInstance(),nullptr, &resource);
    if(!result){
        throw std::runtime_error("failed to create window surface");
    }
}

VulkanSurface::VulkanSurface(WindowContext &context): VulkanResource(context) {
    createSurface(context);
}

VulkanSurface::~VulkanSurface() {
    vkDestroySurfaceKHR(ctx.get_vulkanInstance(), resource, nullptr);
}
