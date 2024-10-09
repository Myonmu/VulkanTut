//
// Created by miska on 2024/10/08.
//

#ifndef BINDVERTEXBUFFER_H
#define BINDVERTEXBUFFER_H
#include <CommandBuffer.h>
#include <VulkanAppContext.h>

#include "CommandBufferCmd.h"

class BindVertexBuffer final: public CommandBufferCmd {
public:
    void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) override;
};

inline void BindVertexBuffer::execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) {
    const VkBuffer vertexBuffers[] = {context.vertexBuffer};
    const VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer,0,1,vertexBuffers,offsets);
}
#endif //BINDVERTEXBUFFER_H
