//
// Created by miska on 2024/10/08.
//

#ifndef BINDDESCRIPTORSET_H
#define BINDDESCRIPTORSET_H
#include <PipelineLayout.h>
#include <VulkanAppContext.h>
#include <vulkan/vulkan_core.h>

#include "CommandBufferCmd.h"
#include "FrameInfo.h"

class CommandBuffer;

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

#endif //BINDDESCRIPTORSET_H
