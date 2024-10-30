//
// Created by miska on 2024/10/23.
//

#pragma once
#include <iostream>
#include <map>
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
