//
// Created by miska on 2024/02/07.
//

#ifndef VULKANTUT_TRIANGLEAPP_H
#define VULKANTUT_TRIANGLEAPP_H

#include <vector>
#include <optional>

#include "BindDescriptorSet.h"
#include "BindIndexBuffer.h"
#include "BindPipeline.h"
#include "BindVertexBuffer.h"
#include "DrawIndexed.h"
#include "EnqueueRenderPass.h"
#include "RenderPassRecorder.h"
#include "SetScissors.h"
#include "SetViewport.h"
#include "GLFW/glfw3.h"
#include "VulkanAppContext.h"


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class TriangleApp {
public :
    void Run() {
        context = new VulkanAppContext(WIDTH, HEIGHT, "Vulkan");
        setup();
        mainLoop();
    }

private:
    VulkanAppContext *context = nullptr;
    RenderPassRecorder *mainPass = nullptr;
    void setup() {
        mainPass = new RenderPassRecorder(context->renderPass);
        mainPass->enqueueCommand<BindPipeline>(VK_PIPELINE_BIND_POINT_GRAPHICS);
        mainPass->enqueueCommand<SetViewport>();
        mainPass->enqueueCommand<SetScissors>();
        mainPass->enqueueCommand<BindVertexBuffer>();
        mainPass->enqueueCommand<BindIndexBuffer>();
        mainPass->enqueueCommand<BindDescriptorSet>();
        mainPass->enqueueCommand<DrawIndexed>(static_cast<uint32_t>(Vertex::testIndices.size()));
        //TODO: stub
        auto& recorder = context->commandBufferRecorder;
        recorder.enqueueCommand<EnqueueRenderPass>(*mainPass);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(context->window)) {
            glfwPollEvents();
            context->drawFrame();
        }

        vkDeviceWaitIdle(context->logicalDevice);
    }
};


#endif //VULKANTUT_TRIANGLEAPP_H
