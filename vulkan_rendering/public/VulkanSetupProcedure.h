//
// Created by miska on 2024/10/17.
//

#ifndef VULKANSETUPPROCEDURE_H
#define VULKANSETUPPROCEDURE_H


struct DeviceContext;

// interface for vulkan initialization
class VulkanSetupProcedure {
public:
    virtual ~VulkanSetupProcedure() = default;
    virtual void createDeviceContexts(VulkanAppContext& appContext) = 0;
};

class VulkanDeviceSetupProcedure {
public:
    virtual ~VulkanDeviceSetupProcedure() = default;
    // called before selecting physical device
    virtual void createWindows(DeviceContext& deviceContext) = 0;
    virtual void createRenderPasses(DeviceContext& deviceContext) = 0;

};


#endif //VULKANSETUPPROCEDURE_H
