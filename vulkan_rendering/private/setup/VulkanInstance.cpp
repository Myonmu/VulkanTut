//
// Created by miska on 2024/02/08.
//

#include "VulkanInstance.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include "ValidationLayers.h"
#include "VulkanAppContext.h"

[[nodiscard]] std::vector<const char *> VulkanInstance::getRequiredExtensions(const VulkanAppContext& context) {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    // ReSharper disable once CppDFAConstantConditions
    if (context.enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

void VulkanInstance::createInstance(VulkanAppContext& context) {
    if (!ValidationLayers::CheckValidationLayerSupport()) {
        throw std::runtime_error("validation layers unavailable");
    }
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = context.name;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "None";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = apiVersion;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto glfwExtensions = getRequiredExtensions(context);

    createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();
    createInfo.enabledLayerCount = 0;


    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    // ReSharper disable once CppDFAConstantConditions
    if (context.enableValidationLayers) {
        ValidationLayers::AttachToInstanceCreation(debugCreateInfo, createInfo);
    } else {
        // ReSharper disable once CppDFAUnreachableCode
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    auto result = vkCreateInstance(&createInfo, nullptr, &resource);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }

}

VulkanInstance::VulkanInstance(VulkanAppContext &context) : VulkanResource(context){
    createInstance(context);
}

VulkanInstance::~VulkanInstance() {
    vkDestroyInstance(resource, nullptr);
}

