//
// Created by miska on 2024/02/07.
//

#ifndef VULKANTUT_TRIANGLEAPP_H
#define VULKANTUT_TRIANGLEAPP_H

#include <vector>
#include <optional>
#include <algorithm>
#include <VulkanRenderer.h>

#include "GLFW/glfw3.h"
#include "VulkanAppContext.h"


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class TriangleApp {
public :
    void Run() {
        context = new VulkanAppContext(WIDTH, HEIGHT, "Vulkan");
        mainLoop();
    }

private:
    VulkanAppContext *context = nullptr;
    void mainLoop() {
        while (!glfwWindowShouldClose(context->window)) {
            glfwPollEvents();
            context->drawFrame();
        }

        vkDeviceWaitIdle(context->logicalDevice->getRaw());
    }
};


#endif //VULKANTUT_TRIANGLEAPP_H
