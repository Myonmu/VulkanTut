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
        // Check if the map is not empty
        if (!descriptorSets.empty()) {
            // Access the last key using rbegin
            return descriptorSets.rbegin()->first;
        } else {
            std::cout << "Map is empty!" << std::endl;
        }
        return 0;
    }

    uint32_t getMaxBindingInSet(const uint32_t setId) {
        // Check if the map is not empty
        if (!descriptorSets[setId].empty()) {
            // Access the last key using rbegin
            return descriptorSets[setId].rbegin()->first;
        } else {
            std::cout << "Map is empty!" << std::endl;
        }
        return 0;
    }

    void merge(ShaderReflectionResult &other);
    std::map<VkDescriptorType, uint32_t> getCountByType();
};
