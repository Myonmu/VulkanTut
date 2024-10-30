//
// Created by miska on 2024/02/11.
//

#pragma once


#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>
#include <vector>
class Shader;
struct DeviceContext;

class ShaderModule : public VulkanResource<VkShaderModule, DeviceContext>{
    VkShaderStageFlagBits stageFlags;
    const char* entryPoint = "main";
public:
    ShaderModule(const std::string& shaderPath, DeviceContext& context, VkShaderStageFlagBits stageFlags);
    ShaderModule(const std::vector<uint32_t>& code ,DeviceContext& context, VkShaderStageFlagBits stageFlags);
    ShaderModule(const Shader& shader, DeviceContext& context);
    ~ShaderModule() override;

    [[nodiscard]] VkPipelineShaderStageCreateInfo getShaderStageCreateInfo() const;
};


