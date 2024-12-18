//
// Created by miska on 2024/02/08.
//

#pragma once


#include <vector>
#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"

struct VulkanAppContext;

class VulkanInstance : public VulkanResource<VkInstance, VulkanAppContext> {
public:
    const uint32_t apiVersion = VK_API_VERSION_1_3;
    explicit VulkanInstance(VulkanAppContext& context);
    ~VulkanInstance() override;
private:
    void createInstance(VulkanAppContext& context);
    [[nodiscard]] static std::vector<const char *> getRequiredExtensions(const VulkanAppContext& context) ;
};

