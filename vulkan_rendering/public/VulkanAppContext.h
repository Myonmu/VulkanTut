//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_VULKANAPPCONTEXT_H
#define VULKANTUT_VULKANAPPCONTEXT_H
#include <CommandPool.h>
#include <DescriptorPool.h>
#include <DescriptorSetLayout.h>
#include <DescriptorSets.h>
#include <FrameBuffers.h>
#include <GlfwWindow.h>
#include <IndexBuffer.h>
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
#include "UniformBufferGroup.h"
#include "VulkanPipeline.h"
#include "GLFW/glfw3.h"


struct VulkanAppContext {
    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const int MAX_FRAMES_IN_FLIGHT = 2;
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    const char *name;
    int width;
    int height;
    GlfwWindow window{*this, width, height, name, frameBufferResizeCallback};

    VulkanInstance vulkanInstance{*this};
    ValidationLayers validationLayers{*this};
    VulkanSurface vulkanSurface{*this};
    PhysicalDevice physicalDevice{*this};
    LogicalDevice logicalDevice{*this};

    // ------------ Logical device created -------------------
    DescriptorSetLayout descriptorSetLayout{
        *this,
        {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        }
    };
    DescriptorPool descriptorPool{*this};
    UniformBufferGroup uniformBufferGroup{*this};
    DescriptorSets descriptorSets{*this};
    SwapChain swapChain{*this};
    GraphicsPipeline graphicsPipeline{*this};
    RenderPass renderPass{*this};
    VulkanPipeline vulkanPipeline{*this};
    FrameBuffers frameBuffers{*this};
    CommandPool commandPool{*this};
    VertexBuffer vertexBuffer{*this};
    IndexBuffer indexBuffer{*this};
    VulkanRenderer renderer{*this};

    CommandBufferRecorder frameDrawer{0};

    VulkanAppContext(int w, int h, const char *appName);

    ~VulkanAppContext();

    void resize(int newWidth, int newHeight);

    static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);

    void resize();

    void drawFrame();
};


#endif //VULKANTUT_VULKANAPPCONTEXT_H
