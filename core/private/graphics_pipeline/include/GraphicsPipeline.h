//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_GRAPHICSPIPELINE_H
#define VULKANTUT_GRAPHICSPIPELINE_H

#include "ShaderModule.h"

struct VulkanAppContext;
class GraphicsPipeline : public VulkanResource<VkPipelineLayout>{
public:
    explicit GraphicsPipeline(VulkanAppContext& context);
    ~GraphicsPipeline() override;
private:
    VulkanAppContext* ctx;
};


#endif //VULKANTUT_GRAPHICSPIPELINE_H
