//
// Created by miska on 2024/08/06.
//
#pragma once

#include <Buffer.h>
#include <VulkanResource.h>

class VertexBuffer {
private:
    VkDeviceSize size;
    DeviceContext &ctx;

public:
    explicit VertexBuffer(DeviceContext &context);

    ~VertexBuffer();

    Buffer buffer;
    operator VkBuffer() const {return buffer;}
};

