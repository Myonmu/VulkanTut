//
// Created by Tereza on 10/22/2024.
//

#pragma once
#include <spirv_cross.hpp>
#include <vector>

#include "DescriptorSetLayoutBinding.h"
#include "ShaderReflectionResult.h"

class Shader {
    spirv_cross::ShaderResources resources;
    void analyzeBinding(const spirv_cross::Compiler& compiler, const spirv_cross::Resource& resource, VkDescriptorType type);
public:
    explicit Shader(std::vector<uint32_t> code, VkShaderStageFlagBits stage);
    ~Shader();

    VkShaderStageFlagBits stage;
    const std::vector<uint32_t> code;
    ShaderReflectionResult reflectionResult{};
    static ShaderReflectionResult getMergedBindings(std::vector<Shader> shaders);
};
