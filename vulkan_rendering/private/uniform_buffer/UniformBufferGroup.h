//
// Created by miska on 2024/08/21.
//

#pragma once

#include <Buffer.h>
#include <vector>


struct VulkanAppContext;

class UniformBufferGroup {
    std::vector<Buffer*> uniformBuffers;
    std::vector<void*> uniformBuffersData;
    DeviceContext& ctx;
public:
    explicit UniformBufferGroup(DeviceContext& ctx);
    Buffer& operator[](size_t index) const;
    ~UniformBufferGroup();
    void CopyMemoryToBuffer(const uint32_t& slot,const void* data, const size_t& size) const;
};


