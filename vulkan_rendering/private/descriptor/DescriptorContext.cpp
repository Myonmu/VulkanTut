//
// Created by Tereza on 10/12/2024.
//

#include "DescriptorContext.h"

#include <VulkanAppContext.h>

#include "DescriptorSetLayoutBinding.h"

DescriptorContext::DescriptorContext(VulkanAppContext &ctx, const std::vector<DescriptorSetLayoutBinding> &bindings)
    : SubContext(ctx),
      descriptorSetLayout(*this, bindings),
      descriptorPool(*this) {
}
