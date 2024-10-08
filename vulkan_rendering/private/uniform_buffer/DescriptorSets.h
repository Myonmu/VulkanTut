//
// Created by miska on 2024/08/27.
//

#ifndef DESCRIPTORSETS_H
#define DESCRIPTORSETS_H
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


class DescriptorSets : public VulkanResource<std::vector<VkDescriptorSet>> {
    void configureDescriptorSets() const;
public:
    explicit DescriptorSets(VulkanAppContext& ctx);
    ~DescriptorSets() override;
    VkDescriptorSet& operator[](size_t index);
};



#endif //DESCRIPTORSETS_H
