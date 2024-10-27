//
// Created by miska on 2024/08/15.
//

#pragma once

#include "vk_mem_alloc.h"

struct DeviceContext;

// TODO: Stub.
class VmaInstance {
    VmaAllocator allocator;

public:
    VmaInstance(DeviceContext &ctx);

    ~VmaInstance();

    operator VmaAllocator() const {
        return allocator;
    }
};
