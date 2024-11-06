//
// Created by Tereza on 10/22/2024.
//

#include "Shader.h"
#include <iostream>
#include "spirv_cross.hpp"

void ShaderReflectionResult::addBinding(const DescriptorSetLayoutBinding &binding) {
    if (descriptorSets.contains(binding.setId) &&
        descriptorSets[binding.setId].contains(binding.binding)
    ) {
        if (descriptorSets[binding.setId][binding.binding] != binding) {
            // same set, same binding, different signature (not allowed)
            std::cerr << "Conflicting descriptor set declaration found at set(" << binding.setId << "), binding("
                    << binding.binding << ")" << std::endl;
        } else {
            // same, with potentially different shader stage
            descriptorSets[binding.setId][binding.binding].stageFlags |= binding.stageFlags;
        }
    } else {
        descriptorSets[binding.setId][binding.binding] = binding;
    }
}


void ShaderReflectionResult::merge(ShaderReflectionResult &other) {
    auto otherMaxSetId = other.getMaxSetId();
    auto thisMaxSetId = getMaxSetId();
    auto maxSetId = std::max(otherMaxSetId, thisMaxSetId);
    for (uint32_t i = 0; i <= maxSetId; ++i) {
        if (other.descriptorSets.contains(i)) {
            if (!descriptorSets.contains(i)) {
                // full transcribe
                descriptorSets[i] = other.descriptorSets[i];
            } else {
                auto &thisSet = descriptorSets[i];
                auto &otherSet = other.descriptorSets[i];
                auto maxBinding = std::max(other.getMaxBindingInSet(i), getMaxBindingInSet(i));
                for (uint32_t j = 0; j <= maxBinding; ++j) {
                    if (!thisSet.contains(j) && otherSet.contains(j)) {
                        // Transcribe missing
                        thisSet[j] = otherSet[j];
                    } else if (thisSet.contains(j) && otherSet.contains(j)) {
                        // Merge properties
                        auto &thisBinding = thisSet[j];
                        auto &otherBinding = otherSet[j];
                        // Sanity check (same binding but different signature)
                        if (thisBinding != otherBinding) {
                            std::cerr << "Attempting to merge conflicting descriptor sets at set(" << i <<
                                    "), binding(" << j << ")" << std::endl;
                        }else {
                            thisBinding.stageFlags |= otherBinding.stageFlags;
                        }
                    }
                }
            }
        }
    }
}

std::map<VkDescriptorType, uint32_t> ShaderReflectionResult::getCountByType() {
    auto result = std::map<VkDescriptorType, uint32_t>();
    for (const auto &layouts: descriptorSets | std::views::values) {
        for (const auto &binding: layouts | std::views::values) {
            if (result.contains(binding.type)) result[binding.type] ++;
            else {
                result[binding.type] = 1;
            }
        }
    }
    return result;
}

void Shader::analyzeBinding(const spirv_cross::Compiler &compiler,
                            const spirv_cross::Resource &resource,
                            const VkDescriptorType type) {
    const spirv_cross::SPIRType &t = compiler.get_type(resource.type_id);
    uint32_t set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
    uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
    reflectionResult.addBinding({
        set, binding, resource.name,t.array.empty() ? 1 : t.array.back(),
        type,
        stage, nullptr
    });
}

Shader::Shader(std::vector<uint32_t> code, const VkShaderStageFlags stage): stage(stage), code(code) {
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
