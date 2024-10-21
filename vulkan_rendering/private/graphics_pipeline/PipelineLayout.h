//
// Created by miska on 2024/02/11.
//

#pragma once


#include "ShaderModule.h"

struct DeviceContext;
class DescriptorSetLayout;

class PipelineLayout : public VulkanResource<VkPipelineLayout, DeviceContext>{
public:
    explicit PipelineLayout(DeviceContext& context, std::vector<VkDescriptorSetLayout> layouts);
    ~PipelineLayout() override;
};


