//
// Created by miska on 2024/08/21.
//

#ifndef DESCRIPTORSETLAYOUT_H
#define DESCRIPTORSETLAYOUT_H
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "DescriptorSetLayoutBinding.h"


class DescriptorSetLayout:public VulkanResource<VkDescriptorSetLayout> {
public:
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
    explicit DescriptorSetLayout(VulkanAppContext& ctx, const std::vector<DescriptorSetLayoutBinding> &bindings);
    ~DescriptorSetLayout() override;
};



#endif //DESCRIPTORSETLAYOUT_H
