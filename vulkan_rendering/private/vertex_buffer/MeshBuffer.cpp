//
// Created by miska on 2024/11/03.
//

#include "MeshBuffer.h"

#include "Vertex.h"

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
    stagingBuffer->copyToBufferMemory(vertices.data(), 0);
    stagingBuffer->copyToBufferMemory(indices.data(), indexOffset);
    Buffer::copyBuffer(*stagingBuffer, *buffer, ctx, size);
    if (oneTimeStaging) {
        stagingBuffer.reset();
    }
}
