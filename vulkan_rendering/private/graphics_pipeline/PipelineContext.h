//
// Created by miska on 2024/10/13.
//

#ifndef PIPELINECONTEXT_H
#define PIPELINECONTEXT_H
#include <PipelineLayout.h>
#include <RenderPass.h>
#include <VulkanPipeline.h>

#include "SubContext.h"


struct VulkanAppContext;

struct PipelineContext : public SubContext<VulkanAppContext>{
    PipelineLayout graphicsPipeline{*this}; //TODO: allow instance
    RenderPass renderPass{*this}; // TODO: allow instance
    VulkanPipeline vulkanPipeline{*this}; //TODO: allow instance
};



#endif //PIPELINECONTEXT_H
