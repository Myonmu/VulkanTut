//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_VALIDATIONLAYERS_H
#define VULKANTUT_VALIDATIONLAYERS_H

#include <vulkan/vulkan_core.h>
#include "VulkanResource.h"

struct VulkanAppContext;

class ValidationLayers : public VulkanResource<VkDebugUtilsMessengerEXT, VulkanAppContext>{
public:
    explicit ValidationLayers(VulkanAppContext& context);
    ~ValidationLayers() override;
    static bool CheckValidationLayerSupport();
    static void AttachToInstanceCreation(VkDebugUtilsMessengerCreateInfoEXT &pExt, VkInstanceCreateInfo &pInfo);

    static void AttachToDeviceCreation(VkDeviceCreateInfo& createInfo);

private:
    static const std::vector<const char *> validationLayers;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                        VkDebugUtilsMessageTypeFlagsEXT msgType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData) ;


    static int32_t Count();
    static const char* const* Names();
    void setupDebugMessenger(const VulkanAppContext& context);
    static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
};


#endif //VULKANTUT_VALIDATIONLAYERS_H
