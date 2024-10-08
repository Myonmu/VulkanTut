//
// Created by miska on 2024/02/07.
//

#ifndef VULKANTUT_TRIANGLEAPP_H
#define VULKANTUT_TRIANGLEAPP_H

#include <vector>
#include <optional>
#include <algorithm>

#include "BindDescriptorSet.h"
#include "BindIndexBuffer.h"
#include "BindVertexBuffer.h"
#include "DrawIndexed.h"
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
        //TODO: stub
        auto& recorder = context->commandBufferRecorder;
        recorder.enqueueCommand<SetViewport>();
        recorder.enqueueCommand<SetScissors>();
        recorder.enqueueCommand<BindVertexBuffer>();
        recorder.enqueueCommand<BindIndexBuffer>();
        recorder.enqueueCommand<BindDescriptorSet>();
        recorder.enqueueCommand<DrawIndexed>();

        mainLoop();
    }

private:
    VulkanAppContext *context = nullptr;
    void mainLoop() {
        while (!glfwWindowShouldClose(context->window)) {
            glfwPollEvents();
            context->drawFrame();
        }

        vkDeviceWaitIdle(context->logicalDevice);
    }
};


#endif //VULKANTUT_TRIANGLEAPP_H
