//
// Created by miska on 2024/10/27.
//

#include "DescriptorPoolAllocator.h"

#include <LogicalDevice.h>

#include "DeviceContext.h"

DescriptorAllocator::DescriptorAllocator(DeviceContext &ctx): ctx(ctx) {

}

DescriptorAllocator::~DescriptorAllocator() = default;


void DescriptorAllocator::init(uint32_t maxSets, std::span<PoolSizeRatio> poolRatios)
{
    ratios.clear();
    for (auto r : poolRatios) {
        ratios.push_back(r);
    }
    createPool(maxSets, poolRatios);
    setsPerPool = maxSets * 1.5; //grow it next allocation
}

void DescriptorAllocator::clear()
{
    for (const auto& p : readyPools) {
        p->reset(0);
    }
    for (const auto& p : fullPools) {
        p->reset(0);
        readyPools.push_back(p);
    }
    fullPools.clear();
}

void DescriptorAllocator::destroy()
{
    readyPools.clear();
    fullPools.clear();
}

std::unique_ptr<DescriptorPool> DescriptorAllocator::getPool()
{
    if (!readyPools.empty()) {
    	auto& p = readyPools.back();
    	readyPools.pop_back();
        return p;
    }
    else {
	    //need to create a new pool
	    auto newPool = createPool(setsPerPool, ratios);

	    setsPerPool *= 1.5;
	    if (setsPerPool > 4092) {
		    setsPerPool = 4092;
	    }
    	return newPool;
    }
}

std::unique_ptr<DescriptorPool> DescriptorAllocator::createPool(uint32_t setCount, std::span<PoolSizeRatio> poolRatios)
{
	std::vector<VkDescriptorPoolSize> poolSizes;
	for (PoolSizeRatio ratio : poolRatios) {
		poolSizes.push_back(VkDescriptorPoolSize{
			.type = ratio.type,
			.descriptorCount = uint32_t(ratio.ratio * setCount)
		});
	}
	return std::make_unique<DescriptorPool>(ctx, poolSizes);
}

DescriptorSets DescriptorAllocator::allocate(DescriptorSetLayout layout, void* pNext)
{
    //get or create a pool to allocate from
    const auto& poolToUse = getPool();
	try {
		DescriptorSets ds{ctx, *poolToUse, layout};
		return ds;
	}catch (DescriptorPoolOutOfMemoryException& e) {
		fullPools.push_back(poolToUse);
        return allocate(layout, pNext);
	}catch (std::exception& e) {
		throw;
	}
}