//
// Created by miska on 2024/10/08.
//

#ifndef DRAWINDEXED_H
#define DRAWINDEXED_H
#include <CommandBuffer.h>

#include "CommandBufferCmd.h"
#include "Vertex.h"


class DrawIndexed : public CommandBufferCmd{
    uint32_t indicesCount;
public:
    explicit DrawIndexed(uint32_t indicesCount): CommandBufferCmd() {
        this->indicesCount = indicesCount;
    }
    void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) override;
};

inline void DrawIndexed::execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) {
    vkCmdDrawIndexed(commandBuffer, indicesCount, 1, 0 , 0, 0);
}


#endif //DRAWINDEXED_H
