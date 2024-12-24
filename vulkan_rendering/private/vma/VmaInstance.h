//
// Created by miska on 2024/08/15.
//

#pragma once
#include "UtilityMacros.h"
#include "vk_mem_alloc.h"

struct DeviceContext;

enum class StagingBufferMode {
    /**
     * Do not create staging buffer. (when you don't need to transfer data from cpu)
     */
    NO_STAGING_BUFFER,
    /**
     * Create a persistently mapped staging buffer (slightly slower)
     */
    PERSISTENT,
    /**
     * Create but not map the staging buffer.
     * This will result in an implicit map memory call whenever you need to transfer data.
     */
    MAP_PER_CALL
};

/**
* base struct for "create info" structs that should be managed by VMA
* e.g. ImageCreateInfo
*/
template<typename T>
struct VmaAllocatedResourceInfo {
    VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO;
    VmaAllocationCreateFlags allocationFlags = 0;
    float priority = 0.f;

    operator VmaAllocationCreateInfo() const{
        VmaAllocationCreateInfo info{};
        info.usage = memoryUsage;
        info.flags = allocationFlags;
        info.priority = priority;
        return info;
    }

    virtual ~VmaAllocatedResourceInfo() = default;

    /**
    * Tells VMA to create a staging buffer.
    * @param isPersistent when true, the resource is persistently mapped to CPU memory
    * @attention conflicts with isGpuOnly().
    */
    T& isStagingBuffer(bool isPersistent) {
        allocationFlags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        if (isPersistent) {
            allocationFlags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }
        return *dynamic_cast<T*>(this);
    }

    T& isGpuOnly() {
        allocationFlags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        priority = 100.f;
        return *dynamic_cast<T*>(this);
    }
};

class VmaInstance;

class VmaAliasableAllocation {
    VmaInstance& instance;
    VmaAllocation allocation;
public:
    VmaAliasableAllocation(VmaInstance& instance, VmaAllocation allocation);
    ~VmaAliasableAllocation();
};

class VmaInstance {
    VmaAllocator allocator;

public:
    explicit VmaInstance(DeviceContext &ctx);
    NO_COPY(VmaInstance)
    ~VmaInstance();

    std::shared_ptr<VmaAliasableAllocation> allocateAliased();

    operator VmaAllocator() const {
        return allocator;
    }
};
