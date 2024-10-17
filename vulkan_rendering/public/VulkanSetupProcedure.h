//
// Created by miska on 2024/10/17.
//

#ifndef VULKANSETUPPROCEDURE_H
#define VULKANSETUPPROCEDURE_H


struct DeviceContext;

// interface for vulkan initialization
class VulkanSetupProcedure {
public:
    // called before selecting physical device
    virtual void CreateWindows(DeviceContext& deviceContext) = 0;
    
};


#endif //VULKANSETUPPROCEDURE_H
