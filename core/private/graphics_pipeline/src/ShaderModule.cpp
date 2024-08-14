//
// Created by miska on 2024/02/11.
//

#include "ShaderModule.h"
#include "VulkanAppContext.h"

ShaderModule::ShaderModule(const std::vector<char>& code , VulkanAppContext& context){
    ctx = &context;
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(context.logicalDevice->getRaw(),
                             &createInfo,
                             nullptr,
                             &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

ShaderModule::~ShaderModule() {
    vkDestroyShaderModule(ctx->logicalDevice->getRaw(), resource, nullptr);
}
