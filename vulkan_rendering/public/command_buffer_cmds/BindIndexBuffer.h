//
// Created by miska on 2024/10/08.
//

#ifndef BINDINDEXBUFFER_H
#define BINDINDEXBUFFER_H
#include <CommandBuffer.h>

#include "CommandBufferCmd.h"

class BindIndexBuffer final: public CommandBufferCmd {
public:
    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo) override {
        vkCmdBindIndexBuffer(commandBuffer, context.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
};

#endif //BINDINDEXBUFFER_H
