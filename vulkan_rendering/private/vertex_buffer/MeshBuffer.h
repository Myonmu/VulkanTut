//
// Created by miska on 2024/11/03.
//

#pragma once
#include <Buffer.h>

#include "DeviceContext.h"


struct Vertex;

class MeshBuffer: public ObjectNode {
    DeviceContext &ctx;

    VkDeviceSize indexOffset;
    VkDeviceSize size;
    uint32_t indicesCount;
    std::unique_ptr<Buffer> buffer;
    std::unique_ptr<Buffer> stagingBuffer;

public:
    MeshBuffer(DeviceContext &ctx, const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,
               bool oneTimeStaging = true);
    [[nodiscard]] VkDeviceSize getIndexOffset() const { return indexOffset; }
    [[nodiscard]] VkDeviceSize getSize() const { return size; }
    [[nodiscard]] uint32_t getIndicesCount() const { return indicesCount; }
    operator VkBuffer() const {
        return *buffer;
    }
};
