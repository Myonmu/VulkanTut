//
// Created by miska on 2024/10/27.
//

#pragma once
#include <DescriptorPool.h>
#include <DescriptorSets.h>
#include <span>

#include "BindDescriptorSet.h"


class DescriptorAllocator {
public:
    struct PoolSizeRatio {
        VkDescriptorType type;
        float ratio;
    };

    DescriptorAllocator(DeviceContext& ctx);
    ~DescriptorAllocator();
    void init(uint32_t initialSets, std::span<PoolSizeRatio> poolRatios);
    void clear();
    void destroy();
    DescriptorSets allocate(DescriptorSetLayout layout, void* pNext = nullptr);
private:
    DeviceContext& ctx;
    std::unique_ptr<DescriptorPool> getPool();
    std::unique_ptr<DescriptorPool> createPool(uint32_t setCount, std::span<PoolSizeRatio> poolRatios);

    std::vector<PoolSizeRatio> ratios;
    std::vector<std::unique_ptr<DescriptorPool>> fullPools;
    std::vector<std::unique_ptr<DescriptorPool>> readyPools;
    uint32_t setsPerPool;
};
