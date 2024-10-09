//
// Created by miska on 2024/08/15.
//

#include "IndexBuffer.h"

#include "Vertex.h"

IndexBuffer::IndexBuffer(VulkanAppContext &ctx)
    : size(sizeof(Vertex::testIndices[0]) * Vertex::testIndices.size()),
      ctx(ctx),
      buffer(
          ctx, size,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT |
          VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      ) {
    auto stagingBuffer = Buffer(
        ctx, size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    stagingBuffer.bindBufferMemory(Vertex::testIndices.data());
    Buffer::copyBuffer(stagingBuffer, buffer, ctx, size);
}

IndexBuffer::~IndexBuffer() = default;
