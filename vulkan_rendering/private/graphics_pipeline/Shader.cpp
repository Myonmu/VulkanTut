//
// Created by Tereza on 10/22/2024.
//

#include "Shader.h"
#include "spirv_cross.hpp"


void Shader::analyzeBinding(const spirv_cross::Compiler &compiler,
                            const spirv_cross::Resource &resource,
                            const VkDescriptorType type) {
    const spirv_cross::SPIRType &t = compiler.get_type(resource.type_id);
    uint32_t set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
    uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    reflectionResult.addBinding({
        set, binding, resource.name,t.array.empty() ? 1 : t.array.back(),
        type,
        static_cast<VkShaderStageFlags>(stage), nullptr
    });
}

Shader::Shader(std::vector<uint32_t> code, const VkShaderStageFlagBits stage): stage(stage), code(code) {
    // TODO: Shader sanity check, make sure shader declarations conform to engine resource allocation strategy
    // TODO: Currently only considering GLSL

    const spirv_cross::Compiler compiler(std::move(code));

    resources = compiler.get_shader_resources();

    for (auto &u: resources.uniform_buffers) {
        analyzeBinding(compiler, u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    }

    for (auto &combinedImageSamplers: resources.sampled_images) {
        analyzeBinding(compiler, combinedImageSamplers, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    }

    // TODO: Reflection for push constants
    for (auto &pcb: resources.push_constant_buffers) {
    }
}

Shader::~Shader() = default;

ShaderReflectionResult Shader::getMergedBindings(std::vector<Shader> shaders) {
    ShaderReflectionResult result{};
    for (auto &shader: shaders) {
        result.merge(shader.reflectionResult);
    }
    return result;
}
