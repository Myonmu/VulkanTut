//
// Created by miska on 2024/08/04.
//

#include "CommandBufferRecorder.h"

#include <VulkanAppContext.h>

#include "CommandBuffer.h"
#include "Vertex.h"

void CommandBufferRecorder::beginRecordCommandBuffer(CommandBuffer* commandBuffer) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;
    if(vkBeginCommandBuffer(commandBuffer->getRaw(), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer..");
    }
}

void CommandBufferRecorder::endRecordCommandBuffer(CommandBuffer *commandBuffer) {
    if(vkEndCommandBuffer(commandBuffer->getRaw())!=VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
}

void CommandBufferRecorder::setViewport(CommandBuffer *commandBuffer, SwapChain *swapChain) {
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain->swapChainExtent.width);
    viewport.height = static_cast<float>(swapChain->swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer->getRaw(), 0,1,&viewport);
}

void CommandBufferRecorder::setScissors(CommandBuffer *commandBuffer, SwapChain *swapChain) {
     VkRect2D scissor{};
    scissor.offset = {0,0};
    scissor.extent = swapChain->swapChainExtent;
    vkCmdSetScissor(commandBuffer->getRaw(), 0, 1, &scissor);
}

void CommandBufferRecorder::bindVertexBuffer(CommandBuffer *commandBuffer, VertexBuffer *vertexBuffer) {
    VkBuffer vertexBuffers[] = {vertexBuffer->vertexBuffer->getRaw()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer->getRaw(),0,1,vertexBuffers,offsets);
}


void CommandBufferRecorder::recordCommandBuffer(VulkanAppContext* context, CommandBuffer* commandBuffer, uint32_t imageId) {
    beginRecordCommandBuffer(commandBuffer);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = context->renderPass->getRaw();
    renderPassInfo.framebuffer = context->frameBuffers->getRaw()[imageId];
    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = context->swapChain->swapChainExtent;

    VkClearValue clearColor = {{{0.0f,0.0f,0.0f,1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer->getRaw(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer->getRaw(), VK_PIPELINE_BIND_POINT_GRAPHICS, context->vulkanPipeline->getRaw());

    setViewport(commandBuffer, context->swapChain);

    setScissors(commandBuffer, context->swapChain);

    bindVertexBuffer(commandBuffer, context->vertexBuffer);

    vkCmdDraw(commandBuffer->getRaw(), static_cast<uint32_t>(Vertex::testVerts.size())
        , 1, 0 , 0);

    vkCmdEndRenderPass(commandBuffer->getRaw());

    endRecordCommandBuffer(commandBuffer);
}
