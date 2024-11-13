//
// Created by miska on 2024/08/04.
//https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Command_buffers#page_Command-buffer-recording
//

#pragma once

#include <CommandBuffer.h>
#include <vector>

#include "CommandBufferCmd.h"

struct FrameInfo;
class VertexBuffer;
class SwapChain;
struct VulkanAppContext;
class CommandBuffer;

class CommandBufferRecorder {

    std::vector<CommandBufferCmd*> commands;

    VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    typedef void (*RecordFunc)(const CommandBuffer &commandBuffer, const DeviceContext &context, const FrameInfo &frameInfo);

    RecordFunc recordFunc = nullptr;

    bool isSubRecorder;

public:
    explicit CommandBufferRecorder(bool isSubRecorder = false);
    explicit CommandBufferRecorder(VkCommandBufferUsageFlags flags, bool isSubRecorder = false);

    void beginRecordCommandBuffer(const CommandBuffer &commandBuffer) const;

    static void endRecordCommandBuffer(const CommandBuffer &commandBuffer);

    // Use this to inject custom recorder (e.g. call raw vulkan api)
    void setRecordFunc(const RecordFunc& f);
    virtual ~CommandBufferRecorder();
    virtual void recordCommandBuffer(const CommandBuffer &commandBuffer, const DeviceContext &context,
                             const FrameInfo &frameInfo) const;

    template<typename T, typename... Args>
    void enqueueCommand(Args&&... args) {
        static_assert(std::is_base_of_v<CommandBufferCmd, T>, "T must be a CommandBufferCmd");
        commands.push_back(new T(std::forward<Args>(args)...));
    }

    void clear();
};

