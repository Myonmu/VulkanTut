//
// Created by miska on 2024/02/09.
//

#pragma once


#include <TextureImage.h>
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
    VkExtent2D swapChainExtent{};
    VkFormat swapChainImageFormat {};
    void recreate();
    friend class ColorAttachment;
    TexturePxDimensions getSwapChainPxDimensions() {
        return { swapChainExtent.width, swapChainExtent.height, 1 };
    }
private:
    uint32_t imageCount;
    void createSwapChain(WindowContext& context);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR& capabilities, WindowContext& context);
};
