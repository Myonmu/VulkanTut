//
// Created by miska on 10/9/2024.
//
#pragma once

#include <RenderPass.h>
#include <vector>

#include "CommandBufferCmd.h"


struct WindowContext;
class CommandBuffer;
struct FrameInfo;

/*
 * Use in conjunction with CommandBufferRecorder.
 * Must enqueue a EnqueueRenderPass command on a command buffer to actually make this work
 */
class RenderPassRecorder {
    RenderPass &renderPass;
    std::vector<std::unique_ptr<CommandBufferCmd>> commands;

public:
    explicit RenderPassRecorder(RenderPass &renderPass);

    ~RenderPassRecorder();

    void recordRenderPass(const CommandBuffer &commandBuffer, const DeviceContext &context,
                          const FrameInfo &frameInfo) const;

    void clear();

    template<typename T, typename... Args>
    void enqueueCommand(Args&&... args) {
        static_assert(std::is_base_of_v<CommandBufferCmd, T>, "T must be a CommandBufferCmd");
        commands.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }
};

