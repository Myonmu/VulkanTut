//
// Created by miska on 2024/08/15.
//
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1003000
#include "VmaInstance.h"
#include "DeviceContext.h"
#include "vk_mem_alloc.h"

VmaAlloc::~VmaAlloc() {
    // when EXCLUSIVE, the memory is likely managed by the object itself through vma calls (e.g. vmaDestroyImage)
    if (type == VmaAllocationType::ALIASED) {
        vmaFreeMemory(instance, allocation);
    }
}


VmaInstance::VmaInstance(DeviceContext &ctx) {
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.physicalDevice = ctx.get_physicalDevice();
    allocatorCreateInfo.device = ctx.getLogicalDevice();
    allocatorCreateInfo.instance = ctx.get_vulkanInstance();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;
    allocatorCreateInfo.vulkanApiVersion = std::min(ctx.get_vulkanInstance().apiVersion,
                                                    ctx.get_physicalDevice().getApiVersion());

    auto result = vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create vma allocator");
    }
}

VmaInstance::~VmaInstance() {
    vmaDestroyAllocator(allocator);
}

std::shared_ptr<VmaAlloc> VmaInstance::createEmptyExclusiveAllocation() {
    return std::make_shared<VmaAlloc>(*this, VmaAllocationType::EXCLUSIVE);
}

std::shared_ptr<VmaAlloc> VmaInstance::allocateAliased() {
    return nullptr;
}


