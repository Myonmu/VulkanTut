### Engine-defined Descriptor Allocation

The first descriptor set (set = 0) is engine-defined, meaning whatever is written in shader **must** match the c++ side definition. All engine defined descriptor sets could be found in `EngineDescriptorDef.cpp`.

*By "engine-defined", it means the data filled in these descriptor sets are automatically managed by the engine.*

To add a new engine defined descriptor binding (an ubo most times):

1. Create a data struct declaring its fields. This will be your descriptor. e.g. `struct { glm::vec4 someField; };`
2. Determine which set, which binding it is. Write accordingly in `EngineDescriptorDef.cpp`. In `EngineDescriptorSetCreateInfo` you need to specify its binding, its contribution to allocator ratio, and the size of the associated buffer. Reminder: descriptor pool allocator allocates descriptors by types, so "contribution to allocator ratio" means increasing the ratio of a specific type of descriptor if the type is already present.
3. Decide how often is the buffer updated. Because of frame overlap (or frames in flight), a per-frame updated resource will need to use `PerFrameDescriptorSet`.
4. Decide when to memcpy into the buffer. For the per-frame descriptor set (set = 0), the update is driven by ecs systems (refer to how camera view projection matrix is transferred to the buffer). You might need to create an ecs component type.
5. Change descriptor set binding call. Currently, it is part of the `MeshRenderer` class.