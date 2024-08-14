//
// Created by miska on 2024/02/08.
//

#include <vector>
#include <set>
#include <stdexcept>
#include "LogicalDevice.h"
#include "QueueFamilyIndices.h"

void LogicalDevice::createLogicalDevice(VulkanAppContext& context){
    QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(context.physicalDevice,
                                                                       context);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
            indices.graphicsFamily.value(),
            indices.presentFamily.value()
    };
    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies){
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(context.deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = context.deviceExtensions.data();

    if(context.enableValidationLayers){
        context.validationLayers.AttachToDeviceCreation(createInfo);
    }else{
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(context.physicalDevice, &createInfo, nullptr, &resource)!=VK_SUCCESS){
        throw std::runtime_error("failed to create logical device.");
    }


    vkGetDeviceQueue(resource, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(resource, indices.presentFamily.value(), 0, &presentQueue);
}

LogicalDevice::LogicalDevice(VulkanAppContext& context) : VulkanResource(context){
    createLogicalDevice(context);
}

LogicalDevice::~LogicalDevice() {
    vkDestroyDevice(resource, nullptr);
}

