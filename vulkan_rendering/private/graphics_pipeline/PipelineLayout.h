//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_GRAPHICSPIPELINE_H
#define VULKANTUT_GRAPHICSPIPELINE_H

#include "ShaderModule.h"

struct VulkanAppContext;
class PipelineLayout : public VulkanResource<VkPipelineLayout, PipelineContext>{
public:
    explicit PipelineLayout(PipelineContext& context);
    ~PipelineLayout() override;
};


#endif //VULKANTUT_GRAPHICSPIPELINE_H
