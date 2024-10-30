//
// Created by miska on 2024/02/08.
//

#pragma once

#include <ValidationLayers.h>
#include <vector>

#include "ContextMacros.h"
#include "VulkanInstance.h"
#include "VulkanSetupProcedure.h"


class LogicalDevice;

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
    std::vector<std::unique_ptr<DeviceContext>> deviceContexts;

    void createDeviceContext(VulkanDeviceSetupProcedure &setupProcedure);

    VulkanAppContext(const char *appName, VulkanSetupProcedure& setupProcedure);

    ~VulkanAppContext();

    LogicalDevice& getLogicalDevice(){throw std::runtime_error("AppContext does not contain single logical device");};
};
