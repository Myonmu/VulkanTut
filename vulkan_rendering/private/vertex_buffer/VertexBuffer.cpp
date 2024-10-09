//
// Created by miska on 2024/08/06.
//

#include "VertexBuffer.h"

#include <VulkanAppContext.h>

#include "Vertex.h"

VertexBuffer::VertexBuffer(VulkanAppContext &context)
    : size(sizeof(Vertex::testVerts[0]) * Vertex::testVerts.size()),
      ctx(context),
      buffer(
          context, size,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      ) {
    auto stagingBuffer = Buffer(
        context, size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    stagingBuffer.bindBufferMemory(Vertex::testVerts.data());

    Buffer::copyBuffer(stagingBuffer, buffer, context, size);
}

VertexBuffer::~VertexBuffer() = default;