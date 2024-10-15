//
// Created by miska on 2024/10/13.
//

#ifndef PIPELINECONTEXT_H
#define PIPELINECONTEXT_H
#include <PipelineLayout.h>
#include <RenderPass.h>
#include <VulkanPipeline.h>

#include "ContextMacros.h"
#include "SubContext.h"


struct VulkanAppContext;

struct PipelineContext : public SubContext<VulkanAppContext> {
    CTX_PROPERTY(PipelineLayout, graphicsPipeline)
    CTX_PROPERTY(RenderPass, renderPass)
    CTX_PROPERTY(VulkanPipeline, vulkanPipeline)
};



#endif //PIPELINECONTEXT_H
