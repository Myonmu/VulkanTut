//
// Created by miska on 2024/08/15.
//

#pragma once

#include <Buffer.h>


struct VulkanAppContext;

class IndexBuffer {
private:
    VkDeviceSize size;
    DeviceContext &ctx;
public:
    explicit IndexBuffer(DeviceContext& ctx);
    ~IndexBuffer();
    Buffer buffer;
    operator VkBuffer() const {return buffer;}
};

