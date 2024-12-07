//
// Created by miska on 2024/08/15.
//

#pragma once
#include "vk_mem_alloc.h"

struct DeviceContext;

class VmaInstance {
    VmaAllocator allocator;

public:
    explicit VmaInstance(DeviceContext &ctx);
    VmaInstance(const VmaInstance &) = delete;
    VmaInstance &operator=(const VmaInstance &) = delete;
    ~VmaInstance();

    operator VmaAllocator() const {
        return allocator;
    }
};
