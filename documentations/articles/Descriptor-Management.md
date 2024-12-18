# Descriptor Set Layout, Descriptor Pool, Descriptor Set

Descriptor related concepts was rather tricky (for us) to understand.

Like Pipelines, *Descriptor Set Layout* act as the mold for *Descriptor Set*. *Descriptor Pool* is where descriptor sets are allocated from.

Yet, a descriptor pool isn't tied to a descriptor set layout... different descriptor sets with different layouts can be allocated from the same descriptor pool. This is, as if you are baking cakes - you can pick the same ingredients (flour, sugar, yeast...) from your cupboard and use them to bake different cakes (or breads), *as long as you can find the required ingredients in the cupboard*. Here, your cupboard is the descriptor pool, and the material list of your recipe acts as the Descriptor Set Layout.

*For example: A shader requires 1 UBO at `layout(set=0,binding=0)`, and 1 combined image sampler at `layout(set=0,binding=1)`. The pool used to allocate the descriptor set for this shader can also allocate a shader with 1 UBO at `layout(set=0,binding=7)` and 1 combined image sampler at `layout(set=0,binding=13)`, because the pool only sees that they both require 1 UBO and 1 combined image sampler.*

Descriptor pools have per-type sizes, and this is supplied with `VkDescriptorPoolSize`. The value of `descriptorCount` means *the pool can only allocate up to this amount of **descriptors** of this type*. Practically, it means the maximum number of descriptor sets you want to allocate multiplied by the sum of the specific type of descriptors in that set.  
*e.g. a set with 2 UBO, and you expect 10 instances, then you supply `.descriptorCount = 2*10;`*

Note the difference between **descriptor** and **descriptor sets**: take UBO as example, an UBO is a **descriptor**, and corresponds to a **descriptor binding** you declare in shader (`layout(set=0,binding=0)`), and multiple UBOs bound at specific positions in a specific set form a **descriptor set** (all `layout` that has `set=0` are part of descriptor set 0).

However, note that not all sets have the same sum per type. Technically, it is possible to allocate a descriptor set with 3 UBOs from a pool that was created with `.descriptorCount = 2*10` UBOs, but you would eventually see, for example, after allocating 8 sets of 2 UBOs and 1 set of 3 UBOs, the pool will be left with only 1 UBO descriptor, and you can no longer allocate either of them. This might not be terrible, but imagine thousands of these pools with "holes", it would be a waste of memory.

We could therefore determine whether a pool is compatible with a specific descriptor set layout:

- A descriptor pool is **compatible** with descriptor set layout if it contains all **types** of descriptors required by descriptor set layout.

And A descriptor pool is **suitable** if:

- For each type of descriptor required by descriptor set layout, the sum of this type of descriptors in the layout is multiple of the pool's corresponding pool size divided by the maximum set count. (Or else, it causes fragmentation).
- For each type of descriptor required by descriptor set layout, the sum of this type of descriptors is less or equal to the free descriptor count of the pool. (Or else would be out of memory).

The first suitable condition looks complex but we can reformulate it: Consider for type $T$, a pool is created with $s*N$ descriptors of this type, where $s$ is the sum of descriptors of this type in a single set, and $N$ the maximum number of descriptor sets. A layout with $n*s$ descriptors of type $T$ would guarantee not leaving a hole in the pool, where $n$ is an arbitrary natural number.

The second condition could be mitigated by simply allocating a new pool. (Though it comes with the pool allocation overhead)

There's an extra parameter that appears in `VkDescriptorPoolCreateInfo`, that is `.maxSets`. This is the maximum number of descriptor sets that can be allocated from the descriptor pool. As you might have noticed, the pool might be out of *descriptors* before reaching the `.maxSets` limit, if we have allocated bigger sets (see the *suitable* condition). But, again, out of memory can be mitigated by creating a new pool.

To sum up the relationship between descriptor pools and descriptor set layouts:

- When using shader reflection, we construct descriptor pools after we determine the descriptor set layouts. The pool sizes are constructed with the *unique types* and *sum of each type and the expected descriptor set count.
- It is preferable to reuse the same descriptor pool if a descriptor layout has the same *type-sum* pairs as the layout from which the pool was created from.
- It is possible to reuse the same descriptor pool for a *compatible* and optionally *suitable* descriptor set layout without strictly matching the *type-sum* pairs. But doing so would likely result in out of memory or pool fragmentation.
- Though it is tempting to say that a pool is associated with one `VkPipelineLayout`, but recall that a pipeline can have multiple different set layouts, and set layouts from different pipelines can be the same, therefore this simplified statement is true for simple cases.
- The general rule of determine the relationship between descriptor set layouts and descriptor pools, would be to ask *when can I reuse an existing descriptor set layout?*, following the compatibility and suitability conditions.

## How UE does it

Descriptor pool in UE has this structure:

```c++  
class FVulkanDescriptorPool  
{  
public:  
    FVulkanDescriptorPool(FVulkanDevice* InDevice, const FVulkanDescriptorSetsLayout& Layout, uint32 MaxSetsAllocations);  
    ~FVulkanDescriptorPool();  
    // the rest is ignored  
};  
  
```  

And `FVulkanDescriptorSetsLayout` has declaration:

```c++  
struct FVulkanDescriptorSetLayoutEntry  
{  
    VkDescriptorSetLayout Handle = 0;  
    uint32 HandleId = 0;  
};  
  
using FVulkanDescriptorSetLayoutMap = TMap<FVulkanDescriptorSetsLayoutInfo::FSetLayout, FVulkanDescriptorSetLayoutEntry>;  
  
class FVulkanDescriptorSetsLayout : public FVulkanDescriptorSetsLayoutInfo  
{  
public:  
    FVulkanDescriptorSetsLayout(FVulkanDevice* InDevice);  
    ~FVulkanDescriptorSetsLayout();  
    void Compile(FVulkanDescriptorSetLayoutMap& DSetLayoutMap);  
    // the rest is ignored  
}  
```  

Finally, `FSetLayout` is

```c++  
struct FSetLayout  
{  
    TArray<VkDescriptorSetLayoutBinding> LayoutBindings;  
    uint32 Hash;  
    // the rest is ignored  
}  
```  

From these declarations we can see the following procedure:

1. `FSetLayout` is constructed from layout bindings. This info can be retrieved by using shader reflection.
2. The bindings can be combined (sum per type) to create `VkDescriptorSetLayout`
3. Multiple layouts (e.g. from different shader stages) can be combined to create `VkDescriptorPool`. The implementation is in `VulkanPendingState.cpp`, and it shows how UE sums descriptors by type and fills the descriptor pool sizes with $setCount * descriptorSumOfType$ per type.