//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_GRAPHICSPIPELINE_H
#define VULKANTUT_GRAPHICSPIPELINE_H

#include "ShaderModule.h"

struct DeviceContext;
class DescriptorSetLayout;

class PipelineLayout : public VulkanResource<VkPipelineLayout, DeviceContext>{
public:
    explicit PipelineLayout(DeviceContext& context, std::vector<VkDescriptorSetLayout> layouts);
    ~PipelineLayout() override;
};


#endif //VULKANTUT_GRAPHICSPIPELINE_H
