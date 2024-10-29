//
// Created by miska on 2024/02/11.
//

#pragma once


#include "ShaderModule.h"

struct DeviceContext;
class DescriptorSetLayout;

class PipelineLayout : public VulkanResource<VkPipelineLayout, DeviceContext>{
public:
    explicit PipelineLayout(DeviceContext& context, const std::vector<VkDescriptorSetLayout> &layouts);
    //explicit PipelineLayout(DeviceContext& context, const std::vector<DescriptorSetLayout> &layouts);
    ~PipelineLayout() override;
};


