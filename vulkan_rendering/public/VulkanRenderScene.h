//
// Created by Tereza on 10/12/2024.
//

#ifndef VULKANRENDERSCENE_H
#define VULKANRENDERSCENE_H
#include <CommandPool.h>
#include <DescriptorPool.h>
#include <DescriptorSetLayout.h>
#include <DescriptorSets.h>
#include <FrameBuffers.h>
#include <IndexBuffer.h>
#include <PipelineLayout.h>
#include <RenderPass.h>
#include <SwapChain.h>
#include <VertexBuffer.h>
#include <VulkanPipeline.h>
#include <VulkanRenderer.h>

#include "UniformBufferGroup.h"


struct VulkanAppContext;

struct VulkanRenderScene {
    VulkanAppContext &context;
    DescriptorSetLayout descriptorSetLayout{
        *this,
        {
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        }
    }; // TODO: allow instance
    DescriptorPool descriptorPool{*this}; // TODO: allow instance
    UniformBufferGroup uniformBufferGroup{*this}; //TODO: per-object
    DescriptorSets descriptorSets{*this}; //TODO: per-object
    SwapChain swapChain{*this}; //TODO: per-window
    PipelineLayout graphicsPipeline{*this}; //TODO: allow instance
    RenderPass renderPass{*this}; // TODO: allow instance
    VulkanPipeline vulkanPipeline{*this}; //TODO: allow instance
    FrameBuffers frameBuffers{*this}; //TODO: allow instance
    CommandPool commandPool{*this};
    VertexBuffer vertexBuffer{*this}; //TODO: per-object
    IndexBuffer indexBuffer{*this}; //TODO: per-object
    VulkanRenderer renderer{*this}; //TODO: allow instance

    CommandBufferRecorder frameDrawer{0}; //TODO: allow instance
};


#endif //VULKANRENDERSCENE_H
