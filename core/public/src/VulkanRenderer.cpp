//
// Created by miska on 2024/08/05.
//

#include "VulkanRenderer.h"

#include <VulkanAppContext.h>
#include <VulkanFrame.h>

VulkanRenderer::VulkanRenderer(VulkanAppContext &context) {
    this->context = &context;
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
    frames[currentFrame]->drawFrame();
    currentFrame = (currentFrame + 1) % context->MAX_FRAMES_IN_FLIGHT;
}
