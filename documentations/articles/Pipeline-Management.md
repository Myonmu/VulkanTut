# Pipeline Layout, Pipeline, Descriptor Sets  (WIP)

Consider this scenario in *Unity*: When we want to create a *Material*, we first right-click on a shader and then click Create Material. This will add a Material asset in the project. And then, we could assign textures, change numbers - all in the inspector of that material asset. Finally, when the material is dragged on a renderer, it exposes a *Material  
Property Block* that can be used to further override the properties.

Despite the lack of source code evidence, we could make an analogy between the Unity terms and Vulkan terms, or how we can use Vulkan concepts to mimic this behavior.

- **Shader Asset**: the inspector of the shader asset suggests a serialized version of shader's reflection data (*Descriptor Bindings*). This data would allow us to construct a *Descriptor Set Layout* and a *Pipeline Layout*.
- **Material Asset**: *Pipeline* and shared *descriptor sets*.
- **Material Property Block**: per-instance *descriptor sets*.

Though, it isn't always true that between *Shader Asset* and *Descriptor Set Layout/Pipeline Layout* is a one-to-one relationship. The only thing that can be directly created from shader code is *Shader Modules*.

Different shades can have the same *Descriptor Set Layout* if they have the same layout declarations (we don't care the variable names except for vertex input), and *Pipeline Layout* can be shared if they only differ in the descriptor's content sets or push constants we  
Provide. This means shader code that only differs in execution can use the same pipeline layout.

Now recall that binding pipeline and binding descriptor sets are two distinct function calls: `vkCmdBindPipeline` and `vkCmdBindDescriptorSets`. descriptor sets are bound to *pipeline layout* rather than pipeline (we bind them according to descriptor set layout, and the descriptor set layout is stored in pipeline layout). This further enhances that `VkPipeline` should be shared, because you could bind a pipeline, and then issue different pipelines with different draw calls