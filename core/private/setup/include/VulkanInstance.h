//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_VULKANINSTANCE_H
#define VULKANTUT_VULKANINSTANCE_H

#include <vector>
#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"

struct VulkanAppContext;

class VulkanInstance : public VulkanResource<VkInstance> {
public:
    explicit VulkanInstance(VulkanAppContext& context);
    ~VulkanInstance() override;
private:
    void createInstance(VulkanAppContext& context);
    [[nodiscard]] static std::vector<const char *> getRequiredExtensions(VulkanAppContext& context) ;
};


#endif //VULKANTUT_VULKANINSTANCE_H
