//
// Created by Tereza on 10/11/2024.
//

#pragma once

#include <vulkan/vulkan_core.h>


struct DescriptorSetLayoutBinding {
    uint32_t setId{};
    uint32_t binding{};
    uint32_t descriptorCount{1};
    VkDescriptorType type{};
    VkPipelineStageFlags stageFlags{};
    VkSampler *pImmutableSamplers{nullptr};

    bool operator==(const DescriptorSetLayoutBinding &other) const {
        return other.setId == setId && other.binding == binding
               && other.descriptorCount == descriptorCount
               && other.type == type;
    }

    bool operator!=(const DescriptorSetLayoutBinding &other) const {
        return !(*this == other);
    }
};
