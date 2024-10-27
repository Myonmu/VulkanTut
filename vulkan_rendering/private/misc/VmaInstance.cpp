//
// Created by miska on 2024/08/15.
//
#define VMA_IMPLEMENTATION
#include "VmaInstance.h"

#include "DeviceContext.h"
#include "vk_mem_alloc.h"

VmaInstance::VmaInstance(DeviceContext& ctx) {

    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorCreateInfo.physicalDevice = ctx.get_physicalDevice();
    allocatorCreateInfo.device = ctx.getLogicalDevice();
    allocatorCreateInfo.instance = ctx.get_vulkanInstance();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    VmaAllocator allocator;
    vmaCreateAllocator(&allocatorCreateInfo, &allocator);
}

VmaInstance::~VmaInstance() {
    vmaDestroyAllocator(allocator);
}

