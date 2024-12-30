# Memory Allocation

The project have switched to use VMA (Vulkan Memory Allocator) for memory allocation.

## General Flow

Creating a resource (buffer or image), often require the following steps:

1. Create and fill a *resource descriptor*: either `TextureImageInfo`, `BufferInfo` or `AttachmentInfo`. Resource descriptors are essentially wrappers around the raw vulkan creation structs like `VkImageCreateInfo`, though sometimes with extra fields (e.g. VMA allocation policy).
2. Pay attention to the `.allocationType` in the resource descriptor. This tells the system whether this resource should be created *aliased*. Aliasing means the resource can share the same chunk of memory with other resources, but you can only use one of these resources at any given time. By default, the resource is not aliased.
3. Pass to the resource's constructor. This will always create the underlying vulkan object. When allocation type is `EXCLUSIVE`, the associated memory is also allocated.
4. If the resource is `ALIASED`, then after all aliased resources have been created, pass them to `VmaInstance.allocateAliased(...)` to allocate a shared memory chunk for them.

## Memory Aliasing

As explained earlier, memory aliasing allows multiple resources to share the same chunk of memory. These resources do not need to be of same type (e.g. you could alias a buffer with an image).

The current implementation is not considering more advanced scenario, all aliased memory will be aligned without offset. However, consider this scenario:

```
A: image of size 2 (the size is just a notation)
B: buffer of size 1
C: buffer of size 1

A and B are never used at the same time
A and C are never used at the same time
B and C *could* be used at the same time
```

In this case, we could alias the memory as follows:

```
|---- A ----|
|- B -|- C -|
```

I will leave the implementation of such case for the future.

Aliased memory is referenced by shared pointer, therefore no special operation is needed to free the memory.