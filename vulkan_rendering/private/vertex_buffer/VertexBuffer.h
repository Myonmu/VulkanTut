//
// Created by miska on 2024/08/06.
//

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
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


#endif //VERTEXBUFFER_H
