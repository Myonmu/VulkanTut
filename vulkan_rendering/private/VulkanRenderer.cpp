//
// Created by miska on 2024/08/05.
//

#include "VulkanRenderer.h"

#include <chrono>
#include <VulkanAppContext.h>
#include <VulkanFrame.h>

#include "DeviceContext.h"
#include "WindowContext.h"


VulkanRenderer::VulkanRenderer(WindowContext &context) : ctx(context) {
	const auto maxFrameInFlight = context.context.context.MAX_FRAMES_IN_FLIGHT;
    frames.resize(maxFrameInFlight);
    for (int i = 0; i < maxFrameInFlight; ++i) {
        frames.emplace_back(std::make_unique<VulkanFrame>(context));
    }
    recorder = std::make_unique<CommandBufferRecorder>();
}

VulkanRenderer::~VulkanRenderer() {
    for (auto& element: frames) {
        element.reset();
    }
}

void VulkanRenderer::signalResize() {
    frames[currentFrame]->signalResize();
}

void VulkanRenderer::recordCommandBuffer(const CommandBuffer &command_buffer, const DeviceContext &context, FrameInfo frame_info) const {
    recorder->recordCommandBuffer(command_buffer, context, frame_info);
}


void VulkanRenderer::drawFrame() {
    if (!glfwWindowShouldClose(ctx.get_window())) {
        glfwPollEvents();
        frames[currentFrame]->drawFrame(currentFrame);
        currentFrame = (currentFrame + 1) % ctx.context.context.MAX_FRAMES_IN_FLIGHT;
    }else {
        vkDeviceWaitIdle(ctx.getLogicalDevice());
        ctx.closeWindow();
    }
}
