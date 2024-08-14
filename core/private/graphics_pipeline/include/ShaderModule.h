//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_SHADERMODULE_H
#define VULKANTUT_SHADERMODULE_H


#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>
#include <vector>
class ShaderModule : public VulkanResource<VkShaderModule>{
public:
    ShaderModule(const std::vector<char>& code ,VulkanAppContext& context);
    ~ShaderModule() override;
private:
    VulkanAppContext* ctx;
};


#endif //VULKANTUT_SHADERMODULE_H
