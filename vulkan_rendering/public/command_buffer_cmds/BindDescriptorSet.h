//
// Created by miska on 2024/10/08.
//

#pragma once
#include <DescriptorSets.h>
#include <PipelineLayout.h>
#include <vulkan/vulkan_core.h>

#include "CommandBufferCmd.h"
#include "CommandBuffer.h"
#include "FrameInfo.h"

class BindDescriptorSet final : public CommandBufferCmd {
    PipelineLayout &pipelineLayout;
    DescriptorSets &descriptorSets;

public:
    BindDescriptorSet(PipelineLayout &layout, DescriptorSets &sets): pipelineLayout(layout), descriptorSets(sets) {
    }

    void execute(const CommandBuffer &commandBuffer, const DeviceContext &context,
                 const FrameInfo &frameInfo) override {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout,
                                0, 1, &descriptorSets[frameInfo.currentFrameIndex],
                                0, nullptr);
    }
};

