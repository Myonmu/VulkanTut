//
// Created by Tereza on 10/11/2024.
//

#pragma once

#include <vulkan/vulkan_core.h>


struct DescriptorSetLayoutBinding {
    VkDescriptorType type{};
    VkPipelineStageFlags stageFlags{};
};
