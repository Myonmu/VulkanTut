//
// Created by miska on 2024/02/11.
//

#pragma once


#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>
#include <vector>
struct DeviceContext;

class ShaderModule : public VulkanResource<VkShaderModule, DeviceContext>{
public:
    ShaderModule(std::string shaderPath, DeviceContext& context);
    ShaderModule(const std::vector<char>& code ,DeviceContext& context);
    ~ShaderModule() override;
};


