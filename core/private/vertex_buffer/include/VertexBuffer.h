//
// Created by miska on 2024/08/06.
//

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include <Buffer.h>
#include <VulkanResource.h>

class VertexBuffer{
public:
    explicit VertexBuffer(VulkanAppContext& context);
    ~VertexBuffer();
    Buffer* vertexBuffer;
private:
    VulkanAppContext* context;
};



#endif //VERTEXBUFFER_H
