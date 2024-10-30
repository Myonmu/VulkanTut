//
// Created by miska on 2024/02/11.
//

#include "ShaderModule.h"

#include <FileUtility.h>

#include "DeviceContext.h"
#include "Shader.h"


ShaderModule::ShaderModule(const std::vector<uint32_t> &code, DeviceContext &context,
                           const VkShaderStageFlagBits stageFlags): VulkanResource(context), stageFlags(stageFlags) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size()*(sizeof(uint32_t)/sizeof(char));
    createInfo.pCode = code.data();

    if (vkCreateShaderModule(context.getLogicalDevice(),
                             &createInfo,
                             nullptr,
                             &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

ShaderModule::ShaderModule(const std::string &shaderPath, DeviceContext &context,
                           const VkShaderStageFlagBits stageFlags) : ShaderModule(
    FileUtility::ReadSpv(shaderPath), context, stageFlags) {
}

ShaderModule::ShaderModule(const Shader& shader, DeviceContext &context):
ShaderModule(shader.code, context, shader.stage) {
}

ShaderModule::~ShaderModule() {
    vkDestroyShaderModule(ctx.getLogicalDevice(), resource, nullptr);
}


VkPipelineShaderStageCreateInfo ShaderModule::getShaderStageCreateInfo() const {
    VkPipelineShaderStageCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = stageFlags;
    createInfo.module = resource;
    createInfo.pName = entryPoint;
    return createInfo;
}
