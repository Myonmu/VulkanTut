//
// Created by miska on 2024/02/08.
//



#include <vector>
#include "ValidationLayers.h"

#include <string.h>

#include "VulkanAppContext.h"

// Proxy functions to load not-
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}
const std::vector<const char *> ValidationLayers::validationLayers{
        "VK_LAYER_KHRONOS_validation"
};

bool ValidationLayers::CheckValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    for (const char *layerName: validationLayers) {
        std::cout << layerName << " ----> ";
        bool layerFound = false;
        for (const auto &layerProperties: availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                std::cout << "OK" << std::endl;
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            std::cout << "Not Found" << std::endl;
            return false;
        }
    }
    std::cout << "----- Validation layers OK -----" << std::endl;
    return true;
}

void ValidationLayers::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

void ValidationLayers::setupDebugMessenger(const VulkanAppContext &context) {
    // ReSharper disable once CppDFAConstantConditions
    // ReSharper disable once CppDFAUnreachableCode
    if (!context.enableValidationLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(context.get_vulkanInstance(), &createInfo, nullptr,
                                     &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

int32_t ValidationLayers::Count() {
    return static_cast<int32_t>(validationLayers.size());
}

const char *const *ValidationLayers::Names() {
    return validationLayers.data();
}

void ValidationLayers::AttachToInstanceCreation(
        VkDebugUtilsMessengerCreateInfoEXT &debugCreateInfo,
        VkInstanceCreateInfo &createInfo) {
    createInfo.enabledLayerCount = Count();
    createInfo.ppEnabledLayerNames = Names();
    PopulateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = &debugCreateInfo;
}

void ValidationLayers::AttachToDeviceCreation(VkDeviceCreateInfo &createInfo) {
    createInfo.enabledLayerCount = Count();
    createInfo.ppEnabledLayerNames = Names();
}

ValidationLayers::ValidationLayers(VulkanAppContext &context):VulkanResource(context){
    setupDebugMessenger(context);
}

ValidationLayers::~ValidationLayers() {
    DestroyDebugUtilsMessengerEXT(ctx.get_vulkanInstance(), resource, nullptr);
}

VkBool32 ValidationLayers::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                         VkDebugUtilsMessageTypeFlagsEXT msgType,
                                         const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    {
        if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
            return VK_FALSE;
        }
        return VK_TRUE;
    }
}

