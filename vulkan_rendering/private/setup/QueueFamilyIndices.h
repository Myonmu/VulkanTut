//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_QUEUEFAMILYINDICES_H
#define VULKANTUT_QUEUEFAMILYINDICES_H


#include <optional>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "VulkanAppContext.h"

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const{
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, const VulkanSurface& surface){
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto & queueFamily : queueFamilies){
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR
            (device, i, surface, &presentSupport);
            if(presentSupport){
                indices.presentFamily = i;
            }
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = i;
            }
            if(indices.isComplete()){
                break;
            }
            i++;
        }

        return indices;
    }
};


#endif //VULKANTUT_QUEUEFAMILYINDICES_H
