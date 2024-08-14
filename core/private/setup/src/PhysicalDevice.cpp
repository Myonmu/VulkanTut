//
// Created by miska on 2024/02/08.
//

#include <vector>
#include <map>
#include <iostream>
#include <set>
#include "PhysicalDevice.h"
#include "QueueFamilyIndices.h"
#include "SwapChain.h"

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device, VulkanAppContext& context){
    uint32_t extensionsCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr,
                                         &extensionsCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionsCount);
    vkEnumerateDeviceExtensionProperties( device, nullptr,
                                          &extensionsCount, availableExtensions.data());
    std::set<std::string> requiredExtensions(context.deviceExtensions.begin(), context.deviceExtensions.end());
    for(const auto& extension : availableExtensions){
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}

int PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device, VulkanAppContext& context) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(device, context);
    int score = 0;
    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    if(!indices.isComplete()){
        return 0;
    }

    if(!checkDeviceExtensionSupport(device,context)){
        return 0;
    }else{
        SwapChain::SwapChainSupportDetails swapChainSupport =
                SwapChain::querySwapChainSupport(device,context.vulkanSurface);
        if(swapChainSupport.formats.empty()||swapChainSupport.presentModes.empty()){
            return 0;
        }
    }

    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader) {
        return 0;
    }

    return score;
}

void PhysicalDevice::pickPhysicalDevice(VulkanAppContext& context){
    uint32_t  deviceCount = 0;
    vkEnumeratePhysicalDevices(context.vulkanInstance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(context.vulkanInstance, &deviceCount, devices.data());

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device, context);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        resource = candidates.rbegin()->second;
    } else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(resource, &properties);
    std::cout << "Selected Physical Device : " << properties.deviceName << std::endl;
}

PhysicalDevice::PhysicalDevice(VulkanAppContext &context): VulkanResource<VkPhysicalDevice_T*>(context) {
    pickPhysicalDevice(context);
}
