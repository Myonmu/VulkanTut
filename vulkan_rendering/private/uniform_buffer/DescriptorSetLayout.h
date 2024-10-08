//
// Created by miska on 2024/08/21.
//

#ifndef DESCRIPTORSETLAYOUT_H
#define DESCRIPTORSETLAYOUT_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class DescriptorSetLayout:public VulkanResource<VkDescriptorSetLayout> {
public:
    explicit DescriptorSetLayout(VulkanAppContext& ctx);
    ~DescriptorSetLayout() override;
};



#endif //DESCRIPTORSETLAYOUT_H
