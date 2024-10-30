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
    std::unique_ptr<DescriptorSets> allocate(DescriptorSetLayout &layout, void* pNext = nullptr);
    [[nodiscard]] bool isCompatible(const DescriptorSetLayout& layout) const;
private:
    const float growth = 2;
    DeviceContext& ctx;
    std::unique_ptr<DescriptorPool> getPool();
    std::unique_ptr<DescriptorPool> createPool(uint32_t setCount, std::vector<PoolSizeRatio>& poolRatios);

    std::unordered_set<VkDescriptorType> uniqueTypes;
    std::vector<PoolSizeRatio> ratios;
    std::vector<std::unique_ptr<DescriptorPool>> fullPools;
    std::vector<std::unique_ptr<DescriptorPool>> readyPools;
    uint32_t setsPerPool{};
};
