//
// Created by miska on 2024/10/23.
//

#include "ShaderReflectionResult.h"

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
    auto maxSetId = std::max(other.getMaxSetId(), getMaxSetId());
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