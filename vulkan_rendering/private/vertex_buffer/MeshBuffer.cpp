//
// Created by miska on 2024/11/03.
//

#include "MeshBuffer.h"

#include "Vertex.h"

VertexBuffer::VertexBuffer(DeviceContext &ctx, const std::vector<Vertex> &vertices, bool oneTimeStaging): ctx(ctx),
    size(vertices.size() * sizeof(Vertex)) {
    buffer = std::make_unique<Buffer>(ctx, size,
                                      VK_BUFFER_USAGE_TRANSFER_DST_BIT
                                      | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                      VMA_MEMORY_USAGE_AUTO, 0);
    stagingBuffer = std::make_unique<Buffer>(ctx, size,
                                             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                             VMA_MEMORY_USAGE_CPU_ONLY, 0);
    stagingBuffer->copyToBufferMemory(vertices.data(), 0, size);
    Buffer::copyBuffer(*stagingBuffer, *buffer, ctx, size);
    if (oneTimeStaging) {
        stagingBuffer.reset();
    }
}

IndexBuffer::IndexBuffer(DeviceContext &ctx, const std::vector<uint32_t> &indices,bool oneTimeStaging): ctx(ctx),
    size(indices.size() * sizeof(uint32_t)), indexCount(indices.size())  {
    buffer = std::make_unique<Buffer>(ctx, size,
                                      VK_BUFFER_USAGE_TRANSFER_DST_BIT
                                      | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                      VMA_MEMORY_USAGE_AUTO, 0);
    stagingBuffer = std::make_unique<Buffer>(ctx, size,
                                             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                             VMA_MEMORY_USAGE_CPU_ONLY, 0);
    stagingBuffer->copyToBufferMemory(indices.data(), 0, size);
    Buffer::copyBuffer(*stagingBuffer, *buffer, ctx, size);
    if (oneTimeStaging) {
        stagingBuffer.reset();
    }
}

uint32_t IndexBuffer::getIndicesCount() const {
    return indexCount;
}

MeshBuffer::MeshBuffer(DeviceContext &ctx,
                       const std::vector<Vertex> &vertices,
                       const std::vector<uint32_t> &indices,
                       const bool oneTimeStaging)
    : ctx(ctx),
      indexOffset(sizeof(Vertex) * vertices.size()),
      size(indexOffset + sizeof(uint32_t) * indices.size()),
      indicesCount(indices.size()) {
    buffer = std::make_unique<Buffer>(ctx, size,
                                      VK_BUFFER_USAGE_TRANSFER_DST_BIT
                                      | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                                      VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                      VMA_MEMORY_USAGE_AUTO, 0);
    stagingBuffer = std::make_unique<Buffer>(ctx, size,
                                             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                             VMA_MEMORY_USAGE_CPU_ONLY, 0);
    stagingBuffer->copyToBufferMemory(vertices.data(), 0, indexOffset);
    stagingBuffer->copyToBufferMemory(indices.data(), indexOffset, size - indexOffset);
    Buffer::copyBuffer(*stagingBuffer, *buffer, ctx, size);
    if (oneTimeStaging) {
        stagingBuffer.reset();
    }
}
