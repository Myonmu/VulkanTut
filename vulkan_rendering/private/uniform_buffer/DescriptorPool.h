//
// Created by miska on 2024/08/23.
//

#ifndef DESCRIPTORPOOL_H
#define DESCRIPTORPOOL_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class DescriptorPool: public VulkanResource<VkDescriptorPool>{
public:
    explicit DescriptorPool(VulkanAppContext& ctx);
    ~DescriptorPool() override;
};



#endif //DESCRIPTORPOOL_H
