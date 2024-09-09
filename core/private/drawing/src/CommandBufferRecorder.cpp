//
// Created by miska on 2024/08/04.
//

#include "CommandBufferRecorder.h"

#include <DescriptorSets.h>
#include <VulkanAppContext.h>

#include "CommandBuffer.h"
#include "Vertex.h"

void CommandBufferRecorder::beginRecordCommandBuffer(const CommandBuffer& commandBuffer) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;
    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer..");
    }
}

void CommandBufferRecorder::endRecordCommandBuffer(const CommandBuffer& commandBuffer) {
    if(vkEndCommandBuffer(commandBuffer)!=VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
}

void CommandBufferRecorder::setViewport(const CommandBuffer& commandBuffer, const SwapChain& swapChain) {
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain.swapChainExtent.width);
    viewport.height = static_cast<float>(swapChain.swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0,1,&viewport);
}

void CommandBufferRecorder::setScissors(const CommandBuffer& commandBuffer, const SwapChain& swapChain) {
     VkRect2D scissor{};
    scissor.offset = {0,0};
    scissor.extent = swapChain.swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void CommandBufferRecorder::bindVertexBuffer(const CommandBuffer& commandBuffer, const VertexBuffer& vertexBuffer) {
    const VkBuffer vertexBuffers[] = {(VkBuffer)vertexBuffer.vertexBuffer};
    const VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer,0,1,vertexBuffers,offsets);
}

void CommandBufferRecorder::bindIndexBuffer(const CommandBuffer &commandBuffer, const IndexBuffer &indexBuffer) {
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void CommandBufferRecorder::bindDescriptorSets(const CommandBuffer &commandBuffer,
    const GraphicsPipeline& graphicsPipeline,
    const DescriptorSets& descriptorSets,
    size_t frame) {
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline,
        0, 1, &descriptorSets.getRaw()[frame],
        0, nullptr);
}

void CommandBufferRecorder::recordCommandBuffer(const VulkanAppContext& context,
    const CommandBuffer& commandBuffer, const uint32_t imageId, const uint32_t currentFrameIndex) {
    beginRecordCommandBuffer(commandBuffer);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = context.renderPass;
    renderPassInfo.framebuffer = context.frameBuffers.getRaw()[imageId];
    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = context.swapChain.swapChainExtent;

    VkClearValue clearColor = {{{0.0f,0.0f,0.0f,1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, context.vulkanPipeline);

    setViewport(commandBuffer, context.swapChain);

    setScissors(commandBuffer, context.swapChain);

    bindVertexBuffer(commandBuffer, context.vertexBuffer);
    bindIndexBuffer(commandBuffer, context.indexBuffer);

    bindDescriptorSets(commandBuffer, context.graphicsPipeline, context.descriptorSets, currentFrameIndex);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(Vertex::testIndices.size())
        , 1, 0 , 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    endRecordCommandBuffer(commandBuffer);
}
