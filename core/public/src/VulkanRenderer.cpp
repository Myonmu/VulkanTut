//
// Created by miska on 2024/08/05.
//

#include "VulkanRenderer.h"

#include <chrono>
#include <VulkanAppContext.h>
#include <VulkanFrame.h>


VulkanRenderer::VulkanRenderer(VulkanAppContext &context) : ctx(context){
    frames.resize(context.MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < context.MAX_FRAMES_IN_FLIGHT; ++i) {
        frames[i] = new VulkanFrame(context);
    }
}

VulkanRenderer::~VulkanRenderer() {
    for (auto element: frames) {
        delete element;
    }
}

void VulkanRenderer::signalResize() {
    frames[currentFrame]->signalResize();
}


void VulkanRenderer::drawFrame() {
    frames[currentFrame]->drawFrame(currentFrame);
    currentFrame = (currentFrame + 1) % ctx.MAX_FRAMES_IN_FLIGHT;
}
