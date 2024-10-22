//
// Created by miska on 2024/02/11.
//

#include "ShaderModule.h"

#include <FileUtility.h>

#include "DeviceContext.h"
#include "VulkanAppContext.h"


ShaderModule::ShaderModule(const std::vector<char>& code , DeviceContext& context, VkShaderStageFlagBits stageFlags):
VulkanResource(context), stageFlags(stageFlags){
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(context.getLogicalDevice(),
                             &createInfo,
                             nullptr,
                             &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

ShaderModule::ShaderModule(std::string shaderPath, DeviceContext& context, VkShaderStageFlagBits stageFlags) :
VulkanResource(context), stageFlags(stageFlags){
    auto code = FileUtility::ReadFile(shaderPath);
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(context.getLogicalDevice(),
                             &createInfo,
                             nullptr,
                             &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
                             }
}

ShaderModule::~ShaderModule() {
    vkDestroyShaderModule(ctx.getLogicalDevice(), resource, nullptr);
}


void ShaderModule::fillShaderStageCreateInfo(VkPipelineShaderStageCreateInfo &createInfo) const {
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = stageFlags;
    createInfo.module = resource;
    createInfo.pName = entryPoint;
}
