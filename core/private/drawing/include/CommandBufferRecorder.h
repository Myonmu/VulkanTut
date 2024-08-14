//
// Created by miska on 2024/08/04.
//https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-buffer-recording
//

#ifndef COMMANDBUFFERRECORDER_H
#define COMMANDBUFFERRECORDER_H
#include <CommandBuffer.h>
#include <cstdint>

class VertexBuffer;
class SwapChain;
struct VulkanAppContext;
class CommandBuffer;

class CommandBufferRecorder {
public:
    void recordCommandBuffer(VulkanAppContext *context, CommandBuffer* commandBuffer, uint32_t imageId);
    void beginRecordCommandBuffer(CommandBuffer *commandBuffer);
    void endRecordCommandBuffer(CommandBuffer *commandBuffer);
    void bindVertexBuffer(CommandBuffer* commandBuffer, VertexBuffer* vertexBuffer);
    void setViewport(CommandBuffer *commandBuffer, SwapChain* swapChain);
    void setScissors(CommandBuffer *commandBuffer, SwapChain* swapChain);
};


#endif //COMMANDBUFFERRECORDER_H
