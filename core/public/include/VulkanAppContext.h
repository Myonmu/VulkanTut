//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_VULKANAPPCONTEXT_H
#define VULKANTUT_VULKANAPPCONTEXT_H
#include <CommandPool.h>
#include <FrameBuffers.h>
#include <vector>
#include <VertexBuffer.h>
#include <VulkanRenderer.h>

#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "ValidationLayers.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "LogicalDevice.h"
#include "GraphicsPipeline.h"
#include "RenderPass.h"
#include "VulkanPipeline.h"
#include "GLFW/glfw3.h"


struct VulkanAppContext {
    const int MAX_FRAMES_IN_FLIGHT = 2;
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    const char* name;
    int width;
    int height;
    GLFWwindow* window = nullptr;
    PhysicalDevice* physicalDevice = nullptr;
    LogicalDevice* logicalDevice = nullptr;
    SwapChain* swapChain = nullptr;
    ValidationLayers* validationLayers = nullptr;
    VulkanInstance* vulkanInstance = nullptr;
    VulkanSurface* vulkanSurface = nullptr;
    GraphicsPipeline* graphicsPipeline = nullptr;
    RenderPass* renderPass = nullptr;
    VulkanPipeline* vulkanPipeline = nullptr;
    FrameBuffers* frameBuffers = nullptr;
    CommandPool* commandPool = nullptr;
    VertexBuffer* vertexBuffer = nullptr;
    VulkanRenderer* renderer = nullptr;

    const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VulkanAppContext(int w, int h, const char* appName);

    ~VulkanAppContext();

    void resize(int newWidth, int newHeight);

    static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);

    void resize();

    void drawFrame() const;
};


#endif //VULKANTUT_VULKANAPPCONTEXT_H
