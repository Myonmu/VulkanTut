//
// Created by miska on 2024/08/06.
//

#include "VertexBuffer.h"

#include <VulkanAppContext.h>

#include "Vertex.h"

VertexBuffer::VertexBuffer(DeviceContext &context)
    : size(sizeof(Vertex::testVerts[0]) * Vertex::testVerts.size()),
      ctx(context),
      buffer(
          context, size,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
          VMA_MEMORY_USAGE_AUTO
      ) {
    auto stagingBuffer = Buffer(
        context, size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_ONLY
    );

    stagingBuffer.bindBufferMemory(Vertex::testVerts.data());

    Buffer::copyBuffer(stagingBuffer, buffer, context, size);
}

VertexBuffer::~VertexBuffer() = default;