//
// Created by miska on 2024/08/05.
//

#ifndef COMMANDBUFFERCOMMAND_H
#define COMMANDBUFFERCOMMAND_H

struct FrameInfo;
struct VulkanAppContext;

class CommandBufferCmd {
public:
    virtual ~CommandBufferCmd() = default;
    virtual void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) = 0;
};

#endif //COMMANDBUFFERCOMMAND_H
