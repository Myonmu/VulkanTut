## Overview

TL;DR: The bumpy ride of Vulkan Learning.

The *classic* tutorial, named Vulkan Tutorial is a semi-great place to start coding in Vulkan, but it comes with a horrific code structure: *everything in one file*.

Hence, the repo tries to mitigate this:

- Vulkan objects will be encapsulated in custom wrappers, and usually they have implicit conversion toward the raw Vulkan object. The wrappers follow RAII.
- Long methods are chopped into smaller chunks
- Better interdependencies between objects

We also use Vulkan Guide as supplementary reference.

A homebrew OOP version (more specifically, custom RAII wrapers from scratch), is potentially not optimal compared to the official RAII but should allow us to understand better the inner workings of the API. 

## Topics

### Vulkan Initialization and Surfaces

There is a slightly complex dependency between `VkInstance`, `VkPhysicalDevice`, `VkSurface`, and `VkDevice`, which I believe is not well pronounced in the tutorial. In the tutorial, these are all treated as singletons. However, the only *true singleton* among these objects is `VkInstance`.

Here's how these objects are created in the tutorial: 

```
VkInstance  =>  VkSurface  =>  VkPhysicalDevice  =>  VkDevice
```

This illustration would potentially cause confusion if we were rendering to multiple windows, since for each window, we would need a separate `VkSurface` instance, yet physical device should be shared among these windows, and logical device is one per physical device. But recall *why* we need to use VkSurface in physical device creation in the first place: *querying queue family indices*, and queue family isn't tied to any specific surface, but rather all the surfaces as a whole. In other words, it is the distinct types of queue family used by all surfaces. 

e.g. Surface A requires `VK_QUEUE_GRAPHICS_BIT` and `VK_QUEUE_VIDEO_DECODE_BIT_KHR`, Surface B requires `VK_QUEUE_GRAPHICS_BIT` and `VK_QUEUE_COMPUTE_BIT`, then what we need to pass to physical device selection is the distinct types : `VK_QUEUE_GRAPHICS_BIT`,`VK_QUEUE_VIDEO_DECODE_BIT_KHR` and `VK_QUEUE_COMPUTE_BIT`.

This provides us with a first insight: allocate all the surfaces we might use before physical device selection, so that we know what queue famillies we expect.

```
                VkSurface a  \
                VkSurface b  |         distinct queue family indices
VkInstance  =>      ...      |---------\
                    ...      |          |
                VkSurface n  /          |
                                        |
            ---------------- =>  VkPhysicalDevice  =>  VkDevice
```

Yet there is another problem. What if we want to spawn a new window after logical device creation? 

Theoretically, the newly spawned window (with its own `VkSurface`) might require another queue family that wasn't present in physical device selection or logical device creation. And this would be catastrophic, as there is no way we can dynamically modify physical device nor logical device after their creation. Therefore we would need to reconstruct everything from physical device selection, almost as if you are restarting the whole application.

But how likely would that happen? In most cases, we would expect a dynamically created surface to have queue family requirements already satisfied by the surfaces created before physical device selection. Unless, for example, you *suddenly, unexpectedly* want to encode/decode video in a space shooter. It is very unlikely that you are not sure what you want to do in an application, thus in almost any case, you should be able to determine the queue family indices beforehand.

Given that a dynamically spawned window is almost certain to have the same queue family requirements as previous windows, the newly spawned window will not impact existing physical device and logical device. Though it is always better to be prepared for the worst. Adding an error handling mechanism to validate the queue families required by the new window should suffice, and when there is indeed a new queue family that isn't present, well, we have no choice but to recreate everything from physical device.

As a generic engine, even if we do not need video encoding/decoding at the moment, we could allocate a surface pre-emptively to avoid the hassle when end-user suddenly wants to do so. 

A final note, it is actually possible to not recreate everything, *if we are able to find another physical device that suits the needs*. We could simply run the window on another GPU... But that can cause significant headache afterwards as you might want to synchronize between different devices.

### VkSwapchainKHR, VkRenderPass, and VkFrameBuffer

This is perhaps another understated relationship in the tutorial. 

`VkSwapchainKHR` is effectively per-window, and in the tutorial, `VkRenderPass` is created with a swapchain's image format, and `VkFrameBuffer` requires both a swapchain's image attachments and be bound to **a single render pass**. And this would create a strong dependency chain on our first sight, though uppon closer inspection, not necessarily.

In the tutorial,`VkRenderPass` only requires an image format, but it doesn't state that the image format should come from a swapchain. (Well, actually yes, you *would* use the swapchain's image format, but you don't need to access the swapchain during render pass creation. ) Thus insteand of accessing swapchain's member field, using a constructor injection would decouple the strong relationship between swapchain and render pass. (Note: constructor injection is just a fancy way to say pass the image format as constructor argument)

This means that multiple swapchains can effectively be using the same render pass, if their image format and other bits are compatible. 

Well, not exactly. since swapchains do not *use* a render pass directly. This relationship comes with `VkFrameBuffer` as it uses both a swapchain and a render pass - we must ensure that the swapchain and the render pass are compatible when creating frame buffers, or else, we first need to create a compatible render pass. By "compatible", in general it isn't just the image format, but also store/load operations, etc.

```
VkSwapchainKHR
      \1            VkRenderPass
       \            /1
        \*         /*
        VkFrameBuffer 
```

The compatibility requirements seem strict, but consider the following scenario: a scene is being rendered with multiple windows, using the same device and likely the same render pipeline. For consistency, there's no reason to fiddle with the render pass parameters, thus it is very likely that we can use the same render pass for all these windows. 

### Descriptor Set Layout, Descriptor Pool, Descriptor Set

Descriptor related concepts was rather tricky (for us) to understand. 

Like Pipelines, *Descriptor Set Layout* act as the mold for *Descriptor Set*. *Descriptor Pool* is where descriptor sets are allocated from. 

Yet, a descriptor pool isn't tied to a descriptor set layout... An analogy would be, Descriptor Set Layouts are product blueprints, Descriptor Pools are raw material reserves, and Descriptor Sets are assembled products. Descriptor Pools only care about how many different types of bindings do we have, and doesn't care about the binding's position.

*For example: A shader requires 1 UBO at `layout(set=0,binding=0)`, and 1 combined image sampler at `layout(set=0,binding=1)`. The pool used to allocate the descriptor set for this shader can also be used to allocate a shader with 1 UBO at `layout(set=0,binding=7)` and 1 combined image sampler at `layout(set=0,binding=13)`, because the pool only sees that they both require 1 UBO and 1 combined image sampler.*

Descriptor pools have per-type sizes, and this is supplied with `VkDescriptorPoolSize`. The value of `descriptorCount` means *the pool can only allocate up to this amount of **descriptors** of this type*. Practically, it means the maximum number of descriptor sets you want to allocate multiplied by the sum of the specific type of descriptors in that set. *e.g. a set with 2 UBO, and you expect 10 instances, then you supply `.descriptorCount = 2*10;`*

Note the difference between **descriptor** and **descriptor sets**: take UBO as example, an UBO is a **descriptor**, and corresponds to a **descriptor binding** you declare in shader (`layout(set=0,binding=0)`), and multiple UBOs bound at specific positions in a specific set form a **descriptor set** (all `layout` that has `set=0` are part of descriptor set 0).

However, note that not all sets have the same sum per type. Technically, it is possible to allocate a descriptor set with 3 UBOs from a pool that was created with `.descriptorCount = 2*10` UBOs, but you would eventually see, for example, after allocating 8 sets of 2 UBOs and 1 set of 3 UBOs, the pool will be left with only 1 UBO descriptor, and you can no longer allocate either of them. This might not be terrible, but imagine thousands of these pools with "holes", it would be a waste of memory. 

We could therefore determine whether a pool is compatible with a specific descriptor set layout:

- A descriptor pool is **compatible** with descriptor set layout if it contains all **types** of descriptors required by descriptor set layout.

And A descriptor pool is **suitable** if:

- For each type of descriptor required by descriptor set layout, the sum of this type of descriptors in the layout is multiple of the pool's corresponding pool size divided by the maximum set count. (Or else causes fragmentation)
- For each type of descriptor required by descriptor set layout, the sum of this type of descriptors is less or equal to the free descriptor count of the pool. (Or else would be out of memory).

The first suitable condition looks complex but we can reformulate it: Consider for type $T$, a pool is created with $s*N$ descriptors of this type, where $s$ is the sum of descriptors of this type in a single set, and $N$ the maximum number of descriptor sets. A layout with $n*s$ descriptors of type $T$ would guarantee not leaving a hole in the pool, where $n$ is an arbitrary natural number.

The second condition could be mitigated by simply allocating a new pool. (Though with the pool allocation overhead)

There's an extra parameter that appears in `VkDescriptorPoolCreateInfo`, that is `.maxSets`. This is the maximum number of descriptor sets that can be allocated from the descriptor pool. As you might have noticed, the pool might be out of *descriptors* before reaching the `.maxSets` limit, if we have allocated bigger sets (see the *suitable* condition). But, again, out of memory can be mitigated by creating a new pool.

To sum up the relationship between descriptor pools and descriptor set layouts:

- When using shader reflection, we construct descriptor pools after we determine the descriptor set layouts. The pool sizes are constructed with the *unique types* and *sum of each types* as well as the expected descriptor set count.
- It is preferable to reuse the same descriptor pool if a descriptor layout has the same *type-sum* pairs as the layout from which the pool was created from.
- It is possible to reuse the same descriptor pool for a *compatible* and optionally *suitable* descriptor set layout without strictly matching the *type-sum* pairs. But doing so would likely result in out of memory or pool fragmentation.
- Though it is tempting to say that a pool is associated with one `VkPipelineLayout`, but recall that a pipeline can have multiple different set layouts, and set layouts from different pipelines can be the same, therefore this simplified statement is true for simple cases, but not true in general.
- The general rule of determine the relationship between descriptor set layouts and descriptor pools, would be to ask *when can I reuse an existing descriptor set layout?*, following the compatibility and suitability conditions.

#### How UE does it

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

1. `FSetLayout` is constructed from layout bindings, this info can be retrieved by using shader reflection.
2. The bindings can be combined (sum per type) to create `VkDescriptorSetLayout`
3. Multiple layouts (e.g. from different shader stages) can be combined to create `VkDescriptorPool`. The implementation is in `VulkanPendingState.cpp`, and it shows how UE sums descriptors by type and fill the descriptor pool sizes with $setCount * descriptorSumOfType$ per type.

