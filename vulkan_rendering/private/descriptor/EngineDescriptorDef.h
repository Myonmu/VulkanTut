//
// Created by miska on 2024/11/17.
//

#pragma once
#include <map>
#include "DescriptorPoolAllocator.h"
#include "DescriptorSetLayoutBinding.h"

struct EngineDescriptorSetCreateInfo {
    uint32_t setId{};
    std::map<uint32_t, DescriptorSetLayoutBinding> bindings{};
    std::vector<DescriptorAllocator::PoolSizeRatio> poolSizes{};
    std::map<uint32_t, uint32_t> bufferSizes{};
};

//TODO: This can be serialized?
struct EngineDescriptorDef {
    static std::map<uint32_t, EngineDescriptorSetCreateInfo> descriptorSets;
    static EngineDescriptorSetCreateInfo getSet(uint32_t setId);
};
