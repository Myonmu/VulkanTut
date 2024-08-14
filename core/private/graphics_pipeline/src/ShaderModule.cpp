//
// Created by miska on 2024/02/11.
//

#include "ShaderModule.h"

#include <FileUtility.h>

#include "VulkanAppContext.h"


ShaderModule::ShaderModule(const std::vector<char>& code , VulkanAppContext& context):
VulkanResource<VkShaderModule_T*>(context){
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(context.logicalDevice,
                             &createInfo,
                             nullptr,
                             &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

ShaderModule::ShaderModule(std::string shaderPath, VulkanAppContext& context) : VulkanResource<VkShaderModule_T*>(context){
    auto code = FileUtility::ReadFile(shaderPath);
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(context.logicalDevice,
                             &createInfo,
                             nullptr,
                             &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
                             }
}

ShaderModule::~ShaderModule() {
    vkDestroyShaderModule(ctx.logicalDevice, resource, nullptr);
}
