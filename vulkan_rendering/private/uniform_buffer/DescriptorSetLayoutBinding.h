//
// Created by Tereza on 10/11/2024.
//

#ifndef DESCRIPTORSETLAYOUTBINDING_H
#define DESCRIPTORSETLAYOUTBINDING_H
#include <vulkan/vulkan_core.h>


struct DescriptorSetLayoutBinding {
    VkDescriptorType type{};
    VkPipelineStageFlags stageFlags{};
};



#endif //DESCRIPTORSETLAYOUTBINDING_H
