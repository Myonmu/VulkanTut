//
// Created by miska on 2024/08/15.
//

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
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



#endif //INDEXBUFFER_H
