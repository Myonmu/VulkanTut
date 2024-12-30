### Engine-defined Descriptor Allocation

The first descriptor set (set = 0) is engine-defined, meaning whatever is written in shader **must** match the c++ side definition. All engine defined descriptor sets could be found in `EngineDescriptorDef.cpp`.

*By "engine-defined", it means the data filled in these descriptor sets are automatically managed by the engine.*

To add a new engine defined descriptor binding (an ubo most times):

1. Create a data struct declaring its fields. This will be your descriptor. e.g. `struct { glm::vec4 someField; };`
2. Determine which set, which binding it is. Write accordingly in `EngineDescriptorDef.cpp`. In `EngineDescriptorSetCreateInfo` you need to specify its binding, its contribution to allocator ratio, and the size of the associated buffer. Reminder: descriptor pool allocator allocates descriptors by types, so "contribution to allocator ratio" means increasing the ratio of a specific type of descriptor if the type is already present.
3. Decide how often is the buffer updated. Because of frame overlap (or frames in flight), a per-frame updated resource will need to use `PerFrameDescriptorSet`.
4. Decide when to memcpy into the buffer. For the per-frame descriptor set (set = 0), the update is driven by ecs systems (refer to how camera view projection matrix is transferred to the buffer). You might need to create an ecs component type.
5. Change descriptor set binding call. Currently, it is part of the `MeshRenderer` class.

## Freeform Allocator

*Freeform Allocator* can allocate for any descriptor set layout, but does not ensure optimal usage. Currently, there is one freeform allocator per-frame, and calling `VulkanRenderer.getOrAllocatePerFrameDescriptorSet(...)` will allocate with the freeform allocator of the current frame.

*Freeform Allocator* is backed by `DescriptorAllocatorCollection`, as the name suggests, is a collection of allocators. When calling `allocate(...)`, we check if there is an existing allocator that is compatible with the passed descriptor set layout. If we do find one, we will use it to allocate for the new descriptor set layout. If not, we create a new one with 10 descriptor sets as initial pool size.

"Checking if an existing allocator is compatible with a descriptor set layout" is simply checking whether all types of descriptors in the layout is also present in the allocator. This means if an allocator has more types than the layout, it is considered compatible (but not *optimal*!). When this happens, it is possible that the allocation will cause fragmentation later.

Edit: now the freeform allocator uses a rating system to find the best possible allocator instead of the first compatible allocator. The rating process considers the following criteria:

1. A non-compatible allocator has a score of 0;
2. Higher score when required descriptor type ratio is multiple of the existing ratio.
3. Higher score when required unique descriptor type count matches existing unique type count.

See `DescriptorAllocator::rateCompatibility`.