//
// Created by Tereza on 10/11/2024.
//

#pragma once

#include <vulkan/vulkan_core.h>


struct DescriptorSetLayoutBinding {
    uint32_t setId{0};
    uint32_t binding{0};
    std::string name{};
    uint32_t descriptorCount{1};
    VkDescriptorType type{};
    VkPipelineStageFlags stageFlags{ VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT };
    VkSampler *pImmutableSamplers{nullptr};

    bool operator==(const DescriptorSetLayoutBinding &other) const {
        return other.setId == setId && other.binding == binding
               && other.descriptorCount == descriptorCount
               && other.type == type;
    }

    bool operator!=(const DescriptorSetLayoutBinding &other) const {
        return !(*this == other);
    }

    [[nodiscard]] VkDescriptorSetLayoutBinding asVkDescriptorSetLayoutBinding() const {
        VkDescriptorSetLayoutBinding b{};
        b.binding = binding;
        b.descriptorCount = descriptorCount;
        b.descriptorType = type;
        b.stageFlags = stageFlags;
        b.pImmutableSamplers = pImmutableSamplers;
        return b;
    }

    operator VkDescriptorSetLayoutBinding() const {
        return asVkDescriptorSetLayoutBinding();
    }
};
