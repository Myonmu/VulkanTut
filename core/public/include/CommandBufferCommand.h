//
// Created by miska on 2024/08/05.
//

#ifndef COMMANDBUFFERCOMMAND_H
#define COMMANDBUFFERCOMMAND_H

struct VulkanAppContext;

class CommandBufferCommand {
public:
    void Execute(VulkanAppContext &context);
};

#endif //COMMANDBUFFERCOMMAND_H
