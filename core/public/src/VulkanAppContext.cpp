//
// Created by miska on 2024/08/06.
//

#include <VulkanAppContext.h>

VulkanAppContext::VulkanAppContext(int w, int h, const char *appName) {
    name = appName;
    width = w;
    height = h;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback );
    vulkanInstance = new VulkanInstance(*this);
    validationLayers = new ValidationLayers(*this);
    vulkanSurface = new VulkanSurface(*this);
    physicalDevice = new PhysicalDevice(*this);
    logicalDevice = new LogicalDevice(*this);
    swapChain = new SwapChain(*this);
    graphicsPipeline = new GraphicsPipeline(*this);
    renderPass = new RenderPass(*this);
    vulkanPipeline = new VulkanPipeline(*this);
    frameBuffers = new FrameBuffers(*this);
    commandPool = new CommandPool(*this);
    vertexBuffer = new VertexBuffer(*this);
    renderer = new VulkanRenderer(*this);
}

VulkanAppContext::~VulkanAppContext() {
    delete renderer;
    delete commandPool;
    delete frameBuffers;
    delete vulkanPipeline;
    delete renderPass;
    delete graphicsPipeline;
    delete swapChain;
    delete vertexBuffer;
    delete vulkanSurface;
    delete logicalDevice;
    delete validationLayers;
    delete vulkanInstance;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanAppContext::resize() {
    glfwGetFramebufferSize(window, &width, &height);
    while(width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(logicalDevice->getRaw());
    delete frameBuffers;
    delete swapChain;
    swapChain = new SwapChain(*this);
    frameBuffers = new FrameBuffers(*this);
}

void VulkanAppContext::drawFrame() const {
    renderer->drawFrame();
}


void VulkanAppContext::frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = static_cast<VulkanAppContext*>(glfwGetWindowUserPointer(window));
    app->renderer->signalResize();
}