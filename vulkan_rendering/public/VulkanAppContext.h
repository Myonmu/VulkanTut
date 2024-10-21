//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_VULKANAPPCONTEXT_H
#define VULKANTUT_VULKANAPPCONTEXT_H
#include <DescriptorSetLayout.h>
#include <DescriptorSets.h>
#include <ValidationLayers.h>
#include <vector>
#include <VulkanRenderer.h>

#include "ContextMacros.h"
#include "VulkanInstance.h"
#include "VulkanSetupProcedure.h"


struct VulkanAppContext {
    const int MAX_FRAMES_IN_FLIGHT = 2;
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    VulkanSetupProcedure& setupProcedure;
    const char* name;
    CTX_PROPERTY(VulkanInstance, vulkanInstance)
    CTX_PROPERTY(ValidationLayers, validationLayers)
    std::vector<DeviceContext> deviceContexts;

    template <class T, class... Args>
    void createDeviceContext(Args&&... VAR_ARGS) {
        deviceContexts.emplace_back(*this, &T(std::forward<Args>(VAR_ARGS)...));
    }

    VulkanAppContext(const char *appName, VulkanSetupProcedure& setupProcedure);

    ~VulkanAppContext();

    LogicalDevice& getLogicalDevice(){throw std::runtime_error("AppContext does not contain single logical device");};

    template<class T, class... Args>
    static VulkanAppContext* createAppContext(const char *appName, Args&&... VAR_ARGS) {
        return new VulkanAppContext(appName, &T(std::forward<Args>(VAR_ARGS)...));
    }
};


#endif //VULKANTUT_VULKANAPPCONTEXT_H
