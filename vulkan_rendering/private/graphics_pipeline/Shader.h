//
// Created by Tereza on 10/22/2024.
//

#pragma once
#include <map>
#include <spirv_cross.hpp>
#include <vector>

#include "DescriptorSetLayoutBinding.h"

struct ShaderReflectionResult {
    std::map<uint32_t, std::map<uint32_t, DescriptorSetLayoutBinding> > descriptorSets{};

    void addBinding(const DescriptorSetLayoutBinding &binding);

    uint32_t getMaxSetId() {
        if (!descriptorSets.empty()) {
            return descriptorSets.rbegin()->first;
        }
        return 0;
    }

    uint32_t getMaxBindingInSet(const uint32_t setId) {
        if (descriptorSets.contains(setId) && !descriptorSets[setId].empty()) {
            return descriptorSets[setId].rbegin()->first;
        }
        return 0;
    }

    void merge(ShaderReflectionResult &other);
    std::map<VkDescriptorType, uint32_t> getCountByType();
};


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
