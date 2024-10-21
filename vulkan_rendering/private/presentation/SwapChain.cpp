//
// Created by miska on 2024/02/09.
//

#include <algorithm>
#include <stdexcept>
#include "SwapChain.h"
#include <limits>

#include "DeviceContext.h"
#include "QueueFamilyIndices.h"
#include "WindowContext.h"
#include "GLFW/glfw3.h"

SwapChain::SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface){
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if(formatCount!=0){
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device,surface,&formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    return details;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
    for(const auto& availableFormat : availableFormats){
        if(availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtend(const VkSurfaceCapabilitiesKHR& capabilities, WindowContext& context){
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(context.get_window(), &width, &height);

        VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }
}


void SwapChain::createSwapChain(WindowContext& context){
    auto& physicalDevice = context.get_physicalDevice();
    auto& surface = context.get_surface();
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtend(swapChainSupport.capabilities, context);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = ctx.context.get_queueFamilyIndices();
    const auto size = indices.getQueueFamilyIndicesCount(surface);
    auto* queueFamilyIndices = new uint32_t[size]{};
    indices.fillQueueFamilyIndicesArray(queueFamilyIndices, surface);

    if (indices.getCommonQueueFamilyIndex(QueueFamily::QUEUE_FAMILY_GRAPHICS).value() !=
        indices.getPresentQueueFamilyIndex(surface).value()) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(ctx.getLogicalDevice(), &createInfo, nullptr, &resource) != VK_SUCCESS){
        throw std::runtime_error("failed to create swap chain");
    }

    vkGetSwapchainImagesKHR(ctx.getLogicalDevice(), resource, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(ctx.getLogicalDevice(), resource, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    delete queueFamilyIndices;
}

void SwapChain::createImageViews(){
    swapChainImageViews.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        swapChainImageViews[i] = new ImageView(ctx.context, swapChainImages[i], swapChainImageFormat);
    }
}

SwapChain::~SwapChain() {
    for (const auto imageView : swapChainImageViews) {
        delete imageView;
    }
    vkDestroySwapchainKHR(ctx.getLogicalDevice(), resource, nullptr);
}

SwapChain::SwapChain(WindowContext &context): VulkanResource(context) {
    createSwapChain(context);
    createImageViews();
}

void SwapChain::recreate() {
    for (const auto imageView : swapChainImageViews) {
        delete imageView;
    }
    vkDestroySwapchainKHR(ctx.getLogicalDevice(), resource, nullptr);
    createSwapChain(ctx);
    createImageViews();
}


size_t SwapChain::getSize() const {
    return swapChainImageViews.size();
}
