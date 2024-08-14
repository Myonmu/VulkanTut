//
// Created by miska on 2024/08/06.
//

#include "VertexBuffer.h"

#include <VulkanAppContext.h>

#include "Vertex.h"

VertexBuffer::VertexBuffer(VulkanAppContext &context) {
    this->context = &context;
    auto const size = sizeof(Vertex::testVerts[0]) * Vertex::testVerts.size();

    auto stagingBuffer = new Buffer(
        context, size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    stagingBuffer->bindBufferMemory(Vertex::testVerts.data());

    vertexBuffer = new Buffer(
        context, size,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    Buffer::copyBuffer(stagingBuffer, vertexBuffer, &context, size);
}

VertexBuffer::~VertexBuffer() {
    delete vertexBuffer;
}

