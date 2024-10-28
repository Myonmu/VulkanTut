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
        auto last = descriptorSets.end();
        --last;
        return last->first;
    }

    uint32_t getMaxBindingInSet(const uint32_t setId) {
        auto last = descriptorSets[setId].end();
        --last;
        return last->first;
    }

    void merge(ShaderReflectionResult &other);
    std::map<VkDescriptorType, uint32_t> getCountByType();
};
