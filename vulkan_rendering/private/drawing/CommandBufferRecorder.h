//
// Created by miska on 2024/08/04.
//https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-buffer-recording
//

#ifndef COMMANDBUFFERRECORDER_H
#define COMMANDBUFFERRECORDER_H
#include <CommandBuffer.h>
#include <vector>

#include "CommandBufferCmd.h"

struct FrameInfo;
class VertexBuffer;
class SwapChain;
struct VulkanAppContext;
class CommandBuffer;

class CommandBufferRecorder {
    static void beginRecordCommandBuffer(const CommandBuffer &commandBuffer);

    static void endRecordCommandBuffer(const CommandBuffer &commandBuffer);

    std::vector<CommandBufferCmd*> commands;

public:
    CommandBufferRecorder() = default;
    ~CommandBufferRecorder();
    void recordCommandBuffer(const VulkanAppContext &context, const CommandBuffer &commandBuffer,
                             const FrameInfo &frameInfo);

    template<typename T, typename... Args>
    void enqueueCommand(Args&&... args) {
        static_assert(std::is_base_of_v<CommandBufferCmd, T>, "T must be a CommandBufferCmd");
        commands.push_back(new T(std::forward<Args>(args)...));
    }
};


#endif //COMMANDBUFFERRECORDER_H
