//
// Created by miska on 2024/10/27.
//

#pragma once
#include <DescriptorPool.h>
#include <DescriptorSets.h>
#include <unordered_set>

class DescriptorAllocator {
public:
    struct PoolSizeRatio {
        // type of the descriptor
        VkDescriptorType type;
        // number of this type of descriptor per set
        float ratio;
    };

    explicit DescriptorAllocator(DeviceContext& ctx);
    ~DescriptorAllocator();
    void init(uint32_t initialSets, std::vector<PoolSizeRatio>& poolRatios);
    void clear();
    void destroy();
    DescriptorSets allocate(DescriptorSetLayout &layout, void* pNext = nullptr);
    [[nodiscard]] bool isCompatible(const DescriptorSetLayout& layout) const;
private:
    const float growth = 2;
    DeviceContext& ctx;
    DescriptorPool& getPool();
    DescriptorPool createPool(uint32_t setCount, std::vector<PoolSizeRatio>& poolRatios);

    std::unordered_set<VkDescriptorType> uniqueTypes;
    std::vector<PoolSizeRatio> ratios;
    std::vector<DescriptorPool> fullPools;
    std::vector<DescriptorPool> readyPools;
    uint32_t setsPerPool{};
};
