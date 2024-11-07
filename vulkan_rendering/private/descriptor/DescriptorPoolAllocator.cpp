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
    readyPools.push_back(createPool(initialSets, poolRatios));
    setsPerPool = initialSets * growth; //grow it next allocation
}

void DescriptorAllocator::clear()
{
    for (const auto& p : readyPools) {
        p->reset(0);
    }
    for (auto& p : fullPools) {
        p->reset(0);
        readyPools.push_back(std::move(p));
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
    	auto p = std::move(readyPools.back());
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

std::unique_ptr<DescriptorPool> DescriptorAllocator::createPool(uint32_t setCount, std::vector<PoolSizeRatio>& poolRatios)
{
	std::vector<VkDescriptorPoolSize> poolSizes;
	for (PoolSizeRatio ratio : poolRatios) {
		poolSizes.push_back(VkDescriptorPoolSize{
			.type = ratio.type,
			.descriptorCount = static_cast<uint32_t>(ratio.ratio * setCount)
		});
	}

	return std::make_unique<DescriptorPool>(ctx, setCount, poolSizes);
}

bool DescriptorAllocator::isCompatible(const DescriptorSetLayout &layout) const {
	for (auto &requirements = layout.requirements;
	     const auto &key: requirements | std::views::keys) {
		if (!uniqueTypes.contains(key))return false;
	}
	return true;
}


std::unique_ptr<DescriptorSet> DescriptorAllocator::allocate(DescriptorSetLayout& layout, void* pNext)
{
	if (!isCompatible(layout)) {
		throw std::runtime_error("Cannot allocate with this allocator due to incompatible descriptor set types");
	}
    //get or create a pool to allocate from
	auto poolToUse = getPool();
	try {
		auto result =  std::make_unique<DescriptorSet>(ctx, *poolToUse, layout);
		readyPools.push_back(std::move(poolToUse));
		return result;
	}catch (DescriptorPoolOutOfMemoryException& e) {
		fullPools.push_back(std::move(poolToUse));
        return allocate(layout, pNext);
	}catch (std::exception& e) {
		throw;
	}
}