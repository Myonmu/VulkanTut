//
// Created by miska on 2024/10/23.
//

#pragma once
#include <iostream>
#include <map>
#include "DescriptorSetLayoutBinding.h"


struct ShaderReflectionResult {
    std::map<uint32_t, std::map<uint32_t, DescriptorSetLayoutBinding> > descriptorSets{};

    void addBinding(const DescriptorSetLayoutBinding &binding) {
        if (descriptorSets.contains(binding.setId) && descriptorSets[binding.setId][binding.binding] != binding
        ) {
            std::cout << "Conflicting descriptor set declaration found at (" << binding.setId << "," << binding.binding
                    << ")" << std::endl;
        }
        descriptorSets[binding.setId][binding.binding] = binding;
    }

    ShaderReflectionResult operator|(const ShaderReflectionResult &a, const ShaderReflectionResult &b) {
        ShaderReflectionResult result{};
    }
};
