//
// Created by miska on 2024/08/15.
//

#include "IndexBuffer.h"

#include "Vertex.h"

IndexBuffer::IndexBuffer(DeviceContext &ctx)
    : size(sizeof(Vertex::testIndices[0]) * Vertex::testIndices.size()),
      ctx(ctx),
      buffer(
          ctx, size,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT |
          VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
          VMA_MEMORY_USAGE_AUTO
      ) {
    auto stagingBuffer = Buffer(
        ctx, size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_ONLY
    );

    stagingBuffer.bindBufferMemory(Vertex::testIndices.data());
    Buffer::copyBuffer(stagingBuffer, buffer, ctx, size);
}

IndexBuffer::~IndexBuffer() = default;
