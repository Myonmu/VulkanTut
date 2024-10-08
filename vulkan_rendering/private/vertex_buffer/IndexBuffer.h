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
    VulkanAppContext &ctx;
public:
    explicit IndexBuffer(VulkanAppContext& ctx);
    ~IndexBuffer();
    Buffer indexBuffer;
    operator VkBuffer() const {return indexBuffer;}
};



#endif //INDEXBUFFER_H
