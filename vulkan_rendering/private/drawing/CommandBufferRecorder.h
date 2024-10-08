//
// Created by miska on 2024/08/04.
//https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-buffer-recording
//

#ifndef COMMANDBUFFERRECORDER_H
#define COMMANDBUFFERRECORDER_H
#include <CommandBuffer.h>
#include <cstdint>
#include <DescriptorSets.h>
#include <GraphicsPipeline.h>
#include <IndexBuffer.h>

class VertexBuffer;
class SwapChain;
struct VulkanAppContext;
class CommandBuffer;

class CommandBufferRecorder {
public:
    void recordCommandBuffer(const VulkanAppContext &context, const CommandBuffer &commandBuffer, uint32_t imageId, uint32_t currentFrameIndex);

    void beginRecordCommandBuffer(const CommandBuffer &commandBuffer);

    void endRecordCommandBuffer(const CommandBuffer &commandBuffer);

    void bindVertexBuffer(const CommandBuffer &commandBuffer, const VertexBuffer &vertexBuffer);

    void setViewport(const CommandBuffer &commandBuffer, const SwapChain &swapChain);

    void setScissors(const CommandBuffer &commandBuffer, const SwapChain &swapChain);

    void bindIndexBuffer(const CommandBuffer &commandBuffer, const IndexBuffer &indexBuffer);

    void bindDescriptorSets(const CommandBuffer &commandBuffer, const GraphicsPipeline &graphicsPipeline,
                            const DescriptorSets &descriptorSets, size_t frame);
};


#endif //COMMANDBUFFERRECORDER_H
