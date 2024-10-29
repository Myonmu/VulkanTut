//
// Created by miska on 2024/10/27.
//

#include "DescriptorPoolAllocator.h"

#include <DescriptorSetLayout.h>
#include <ranges>

#include "DeviceContext.h"

DescriptorAllocator::DescriptorAllocator(DeviceContext &ctx): ctx(ctx) {

}

DescriptorAllocator::~DescriptorAllocator() = default;


void DescriptorAllocator::init(uint32_t initialSets, std::vector<PoolSizeRatio>& poolRatios)
{
    ratios.clear();
	uniqueTypes.clear();
    for (auto r : poolRatios) {
        ratios.push_back(r);
    	uniqueTypes.insert(r.type);
    }
    createPool(initialSets, poolRatios);
    setsPerPool = initialSets * growth; //grow it next allocation
}

void DescriptorAllocator::clear()
{
    for (const auto& p : readyPools) {
        p.reset(0);
    }
    for (const auto& p : fullPools) {
        p.reset(0);
        readyPools.push_back(p);
    }
    fullPools.clear();
}

void DescriptorAllocator::destroy()
{
    readyPools.clear();
    fullPools.clear();
}

DescriptorPool& DescriptorAllocator::getPool()
{
    if (!readyPools.empty()) {
    	auto& p = readyPools.back();
    	readyPools.pop_back();
        return p;
    }
    else {
	    //need to create a new pool
	    auto newPool = createPool(setsPerPool, ratios);

	    setsPerPool *= growth;
	    if (setsPerPool > 4092) {
		    setsPerPool = 4092;
	    }
    	return newPool;
    }
}

DescriptorPool DescriptorAllocator::createPool(uint32_t setCount, std::vector<PoolSizeRatio>& poolRatios)
{
	std::vector<VkDescriptorPoolSize> poolSizes;
	for (PoolSizeRatio ratio : poolRatios) {
		poolSizes.push_back(VkDescriptorPoolSize{
			.type = ratio.type,
			.descriptorCount = static_cast<uint32_t>(ratio.ratio * setCount)
		});
	}
	return DescriptorPool{ctx, setCount, poolSizes};
}

bool DescriptorAllocator::isCompatible(const DescriptorSetLayout &layout) const {
	for (auto &requirements = layout.requirements;
	     const auto &key: requirements | std::views::keys) {
		if (!uniqueTypes.contains(key))return false;
	}
	return true;
}


DescriptorSets DescriptorAllocator::allocate(DescriptorSetLayout& layout, void* pNext)
{
	if (!isCompatible(layout)) {
		throw std::runtime_error("Cannot allocate with this allocator due to incompatible descriptor set types");
	}
    //get or create a pool to allocate from
	DescriptorPool &poolToUse = getPool();
	try {
		DescriptorSets a(ctx, poolToUse, layout);
		return a;
	}catch (DescriptorPoolOutOfMemoryException& e) {
		fullPools.push_back(poolToUse);
        return allocate(layout, pNext);
	}catch (std::exception& e) {
		throw;
	}
}