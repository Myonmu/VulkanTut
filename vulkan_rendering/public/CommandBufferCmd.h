//
// Created by miska on 2024/08/05.
//

#ifndef COMMANDBUFFERCOMMAND_H
#define COMMANDBUFFERCOMMAND_H
struct WindowContext;
class CommandBuffer;
struct FrameInfo;
class CommandBufferCmd {
public:
    virtual ~CommandBufferCmd() = default;
    virtual void execute(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo) = 0;
};

#endif //COMMANDBUFFERCOMMAND_H
