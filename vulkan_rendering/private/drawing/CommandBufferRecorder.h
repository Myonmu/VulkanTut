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

    VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    typedef void (*RecordFunc)(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo);

    RecordFunc recordFunc = nullptr;

public:
    CommandBufferRecorder();
    explicit CommandBufferRecorder(VkCommandBufferUsageFlags flags);

    // Use this to inject custom recorder (e.g. call raw vulkan api)
    void setRecordFunc(const RecordFunc& f);
    virtual ~CommandBufferRecorder();
    virtual void recordCommandBuffer(const CommandBuffer &commandBuffer, const VulkanAppContext &context,
                             const FrameInfo &frameInfo) const;

    template<typename T, typename... Args>
    void enqueueCommand(Args&&... args) {
        static_assert(std::is_base_of_v<CommandBufferCmd, T>, "T must be a CommandBufferCmd");
        commands.push_back(new T(std::forward<Args>(args)...));
    }
};


#endif //COMMANDBUFFERRECORDER_H