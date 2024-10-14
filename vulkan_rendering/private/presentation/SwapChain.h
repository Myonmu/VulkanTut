//
// Created by miska on 2024/02/09.
//

#ifndef VULKANTUT_SWAPCHAIN_H
#define VULKANTUT_SWAPCHAIN_H


#include <vulkan/vulkan_core.h>
#include <vector>

#include "ImageView.h"
#include "VulkanResource.h"

struct WindowContext;
struct VulkanAppContext;

class SwapChain : public VulkanResource<VkSwapchainKHR, WindowContext> {
public:
    struct SwapChainSupportDetails{
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    explicit SwapChain(WindowContext& context);
    ~SwapChain() override;
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    [[nodiscard]] size_t getSize() const;
    VkExtent2D swapChainExtent{};
    VkFormat swapChainImageFormat {};
    std::vector<ImageView *> swapChainImageViews;
    void recreate();
private:
    std::vector<VkImage> swapChainImages;

    void createSwapChain(WindowContext& context);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR& capabilities, WindowContext& context);
    void createImageViews();
};


#endif //VULKANTUT_SWAPCHAIN_H
