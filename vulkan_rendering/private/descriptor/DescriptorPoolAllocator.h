//
// Created by miska on 2024/10/27.
//

#pragma once
#include <DescriptorPool.h>
#include <DescriptorSet.h>
#include <DescriptorSetLayout.h>
#include <unordered_set>

class DescriptorAllocator {
public:
    struct PoolSizeRatio {
        // type of the descriptor
        VkDescriptorType type;
        // number of this type of descriptor per set
        float ratio;
    };

    explicit DescriptorAllocator(DeviceContext &ctx);

    ~DescriptorAllocator();

    void init(uint32_t initialSets, std::vector<PoolSizeRatio> &poolRatios);

    void clear();

    void destroy();

    std::unique_ptr<DescriptorSet> allocate(DescriptorSetLayout &layout, void *pNext = nullptr);

    /**
     * Check if this allocator can be used to allocate for the given layout
     * @param layout descriptor set layout
     * @return
     */
    [[nodiscard]] bool isCompatible(const DescriptorSetLayout &layout) const;

    /**
     * Rate this allocator for the given layout. A score of 0 is incompatible.
     * Otherwise, we rate the compatibility by how closely this allocator matches the
     * layout's requirements.
     * @param layout
     * @return
     */
    [[nodiscard]] int rateCompatibility(const DescriptorSetLayout &layout) const;

private:
    const float growth = 2;
    DeviceContext &ctx;

    std::unique_ptr<DescriptorPool> getPool();

    std::unique_ptr<DescriptorPool> createPool(uint32_t setCount, std::unordered_map<VkDescriptorType, PoolSizeRatio> &poolRatios);

    std::unordered_set<VkDescriptorType> uniqueTypes;
    std::unordered_map<VkDescriptorType, PoolSizeRatio> ratios; //TODO: PoolSizeRatio itself can just be a map
    std::vector<std::unique_ptr<DescriptorPool> > fullPools;
    std::vector<std::unique_ptr<DescriptorPool> > readyPools;
    uint32_t setsPerPool{};
};


/**
 * A collection of allocators.
 * Will create a new allocator if a descriptor set is found unsuitable.
 * Not great for performance critical allocations, as it doesn't prevent fragmentation nor optimal pool size.
 */
class DescriptorAllocatorCollection {
    DeviceContext& ctx;
    std::vector<std::unique_ptr<DescriptorAllocator> > allocators;
public:
    explicit DescriptorAllocatorCollection(DeviceContext &ctx);
    std::unique_ptr<DescriptorSet> allocate(DescriptorSetLayout &layout, void *pNext = nullptr);
    void reset() const;
};
