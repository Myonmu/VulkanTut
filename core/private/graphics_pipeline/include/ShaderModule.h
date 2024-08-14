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
    ShaderModule(std::string shaderPath, VulkanAppContext& context);
    ShaderModule(const std::vector<char>& code ,VulkanAppContext& context);
    ~ShaderModule() override;
};


#endif //VULKANTUT_SHADERMODULE_H
