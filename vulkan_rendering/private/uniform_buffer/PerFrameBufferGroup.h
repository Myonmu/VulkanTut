//
// Created by miska on 2024/08/21.
//

#pragma once

#include <Buffer.h>
#include <vector>


struct VulkanAppContext;

class PerFrameBufferGroup {
    std::vector<std::unique_ptr<Buffer>> buffers;
    std::vector<void*> data;
    DeviceContext& ctx;
public:
    explicit PerFrameBufferGroup(DeviceContext& ctx, VkDeviceSize bufferSize);
    Buffer& operator[](size_t index) const;
    ~PerFrameBufferGroup();
    void CopyMemoryToBuffer(const uint32_t& slot,const void* data, const size_t& size) const;
};


