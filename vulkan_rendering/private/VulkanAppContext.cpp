//
// Created by miska on 2024/08/06.
//

#include <VulkanAppContext.h>
#include "DeviceContext.h"

VulkanAppContext::VulkanAppContext(const char *appName, VulkanSetupProcedure &setupProcedure)
    : setupProcedure(setupProcedure), name(appName) {
    vulkanInstance = std::make_unique<VulkanInstance>(*this);
    validationLayers = std::make_unique<ValidationLayers>(*this);
    setupProcedure.createDeviceContexts(*this);
}

VulkanAppContext::~VulkanAppContext() = default;
