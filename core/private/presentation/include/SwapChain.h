//
// Created by miska on 2024/02/09.
//

#ifndef VULKANTUT_SWAPCHAIN_H
#define VULKANTUT_SWAPCHAIN_H


#include <vulkan/vulkan_core.h>
#include <vector>
#include "VulkanResource.h"

struct VulkanAppContext;

class SwapChain : public VulkanResource<VkSwapchainKHR> {
public:
    struct SwapChainSupportDetails{
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    explicit SwapChain(VulkanAppContext& context);
    ~SwapChain() override;
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    [[nodiscard]] size_t getSize() const;
    VkExtent2D swapChainExtent{};
    VkFormat swapChainImageFormat {};
    std::vector<VkImageView> swapChainImageViews;
    void recreate();
private:
    std::vector<VkImage> swapChainImages;

    void createSwapChain(VulkanAppContext& context);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR& capabilities, VulkanAppContext& context);
    void createImageViews(VkDevice logicalDevice);
};


#endif //VULKANTUT_SWAPCHAIN_H
