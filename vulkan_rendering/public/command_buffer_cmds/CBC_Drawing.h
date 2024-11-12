//
// Created by miska on 2024/10/08.
//

#pragma once
#include <DescriptorSet.h>
#include <PipelineLayout.h>
#include <VulkanPipeline.h>
#include <vulkan/vulkan_core.h>
#include "CommandBufferCmd.h"
#include "CommandBuffer.h"
#include "FrameInfo.h"
#include "MeshBuffer.h"

class BindDescriptorSet final : public CommandBufferCmd {
    PipelineLayout &pipelineLayout;
    DescriptorSet &descriptorSets;
    uint32_t setId;

public:
    BindDescriptorSet(PipelineLayout &layout, DescriptorSet &sets, uint32_t setId)
    : pipelineLayout(layout), descriptorSets(sets), setId(setId) {
    }

    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout,
                                setId, 1, descriptorSets,
                                0, nullptr);
    }
};


class BindIndexBuffer : public CommandBufferCmd {
    const IndexBuffer &indexBuffer;
public:
    explicit BindIndexBuffer(const IndexBuffer &vertexBuffer): indexBuffer(vertexBuffer) {
    }
    void execute(const CommandBuffer &commandBuffer,
                 const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
};

class BindVertexBuffer : public CommandBufferCmd {
    const VertexBuffer &vertexBuffer;
public:
    explicit BindVertexBuffer(const VertexBuffer &vertexBuffer): vertexBuffer(vertexBuffer) {
    }
    void execute(const CommandBuffer &commandBuffer,
                 const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        VkBuffer buffer[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffer, offsets);
    }
};


class BindMeshBuffer : public CommandBufferCmd {
    const MeshBuffer &meshBuffer;
public:
    explicit BindMeshBuffer(const MeshBuffer &meshBuffer): meshBuffer(meshBuffer) {
    }
    void execute(const CommandBuffer &commandBuffer,
                 const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        VkBuffer buffer[] = {meshBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffer, offsets);
        vkCmdBindIndexBuffer(commandBuffer, meshBuffer, meshBuffer.getIndexOffset(), VK_INDEX_TYPE_UINT32);
    }
};

class BindPipeline final : public CommandBufferCmd{
    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    VulkanPipeline& pipeline;
public:
    explicit BindPipeline(VulkanPipeline& pipeline, VkPipelineBindPoint bindPoint) : bindPoint(bindPoint), pipeline(pipeline) {}

    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo) override {
        vkCmdBindPipeline(commandBuffer, bindPoint, pipeline);
    }
};

class DrawIndexed : public CommandBufferCmd{
    uint32_t indicesCount;
public:
    explicit DrawIndexed(uint32_t indicesCount): CommandBufferCmd() {
        this->indicesCount = indicesCount;
    }
    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo) override {
        vkCmdDrawIndexed(commandBuffer, indicesCount, 1, 0 , 0, 0);
    }
};


class SetScissors final : public CommandBufferCmd {
public:
    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = context.get_windowContext_at(frameInfo.windowId).get_swapChain().swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }
};

class SetViewport final : public CommandBufferCmd {
public:
    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        auto& swapChain = context.get_windowContext_at(frameInfo.windowId).get_swapChain();
        auto [width, height] = swapChain.swapChainExtent;
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(width);
        viewport.height = static_cast<float>(height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    }
};

class PushConstants final: public CommandBufferCmd {
    PipelineLayout& pipelineLayout;
    VkShaderStageFlags stage{};
    uint32_t size;
    uint32_t offset;
    void* data;
public:

    PushConstants(PipelineLayout& pipelineLayout, VkShaderStageFlags stage, uint32_t size, uint32_t offset, void* data):
    CommandBufferCmd(),
    pipelineLayout(pipelineLayout), data(data), size(size), offset(offset), stage(stage) {}

    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo) override {
        vkCmdPushConstants(commandBuffer, pipelineLayout, stage, offset, size, data);
    }
};
