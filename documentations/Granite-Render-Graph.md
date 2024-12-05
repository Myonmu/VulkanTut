# Render Graph  (WIP)

When Unity first launched Render Graph to replace the old SRP API, it was quite scary to write. I was never so certain why would I need to specify the resources I need to access in what way before calling rendering code? Actually, when I heard about Render Graph, I thought it was actually a graphical interface like Shader Graph.

After diving into vulkan and its Render Passes and Subpasses, I saw the whole picture. What Render Graph does, essentially, is automatically resolving dependencies between passes (synchronization) and group sub passes into a render  
pass.

Now, the primary source of this knowledge comes from [the famous 2017 post](https://themaister.net/blog/2017/08/15/render-graphs-and-vulkan-a-deep-dive/), informative,  
But also, the code is 3k lines long which is slightly daunting. To facilitate comprehension, I decided to break this down into... more bite-size pieces. This might not be entirely correct because of my still-limited knowledge.

## Reformulation of Goals

So, what should a render graph system achieve?

- A way to specify a *Subpass*, with its *input* and *output* *usages*, its *execution order* and a *render function* telling what the subpass should do. Here, *Subpass* is both a Vulkan lingo and Unity URP lingo. Also, the *execution order* of a subpass is often less pronounced in articles I have read, as they use declaration order. In Unity, however, a serialized property is used (e.g. "Before GBuffer")
- Manage resource allocations automatically, with the option to *import* externally allocated resources. To achieve this, the *input* and *output* usages mentioned above should be decoupled from the actual resource (means you don't provide raw Vulkan handles as usage information). Instead, they should be a custom data type that contains enough information for us to allocate the required resources later. And *import* would simply mean a process to bind an actual resource to the custom data type and remove it from render graph's auto-management system.
- Deduct the proper usage of resources, so that when specifying input and output usages, we only need to consider a small subset of the resource's properties, such as size, memory access (read/write), format, etc. An example of deducible property is `StoreOp`, which you could also verify in Unity: If a resource is accessed later in the frame and there's no way to combine it with a pass that writes to the resource, the StoreOp of the resource in the writing pass would become `Store`. And if it isn't used later, StoreOp becomes `Don't Care`. But, obviously, figure out what properties can be deducted is tricky and often requires thinking about the relationship between passes.
- Automatically create *Render Passes* by grouping *Subpasses*.
- Automatically generate *barriers*.

To summarize, these requirements mean:

- Data structure of `RenderGraphNode`, and `RenderGraph`.
- Data structure of `AttachmentUsageDeclaration`, and a way to use this declaration to allocate resources (similar to RTHandles). Note that the Declaration might not necessarily be the datatype we use directly in allocation, it could be  
  converted into an `AttachmentAllocInfo` if that makes more sense.
- Figure out what can be deducted from subpass relationships. (This would mean `AttachmentUsageDeclaration` will only  have the necessary fields while `AttachmentAllocInfo` would have deduced fields).
- Find the *Merge Condition* of the subpasses. (Resolve render graph into vulkan render passes)
- Find *when to add synchronization*.

## Subpass Grouping

The underlying logic in subpass Grouping is obtained from observing the raw Vulkan structs - Render Passes and Subpasses only care about *Frame Buffers*, namely, *Attachment output, Depth Stencil, Resolve, and Input Attachments*. There's a distinction between *Texture* and *Attachment* - they physically mean the same thing (`VkImage`, `VkImageView`), but an *Attachment* is used by `VkFrameBuffer` while *Texture* is more generic.

Therefore in Granite, a distinction is made between *Pass Dependency* and *Pass Merge Dependency*. *Pass Dependency* would account for many resources, and *Pass Merge Dependency* is only built from frame buffer resources.

Narrowing down the candidates isn't enough, because nothing stops us from creating a single render pass with all the textures we need and spam subpasses.

Render Pass and Subpasses are essentially there for Tile Based Rendering, which is commonly seen on mobile devices. When we designate something as framebuffer attachment, the frame buffers are cut into small tiles and each tile would be rendered separately in a render pass.

The first constraint that can prevent us from doing this is this phrase in the Vulkan Specs (Chapter 8):

    The subpasses in a render pass all render to the same dimensions, and fragments for pixel (x,y,layer) in one subpass can only read attachment contents written by previous subpasses at that same (x,y,layer) location.  
It means all attachments that we use in a render pass should have the same dimensions (width, height, layer). Because if we don't, then we cannot cut the attachments equally.

This extends to a second constraint. A texture that is read as input of a subpass is not accessed as what we usually do (by sampling), but with a special syntax both in declaration and reading the data:

```glsl  
layout (input_attachment_index = 0, binding = 0) uniform subpassInput inputPosition;  
  
void main()  
{  
    vec3 fragPos = subpassLoad(inputPosition).rgb;  
}  
```  

Why can't we simply bind it to a descriptor and sample it as other normal textures? - Since tiles are rendered separately, we do not have a guarantee that they all finish rendering at the same moment, nor are they able to be sampled. And if we introduce inter-tile synchronization, we might just not have tile based rendering at all. Therefore, *randomly accessing (`texture(texSampler, fragTexCoord)`)* a framebuffer attachment is impossible. The only way to do it is to exit the render pass, convert the attachment into a sampled texture, and use it in a subsequent render pass.

To sum up:

- Only *Attachments* are concerned with subpass grouping.
- All attachments in a Render Pass must have the same dimensions (width, height, layers).
- No random access to the attachments is allowed.

Note that I avoided the term "Subpass Merging" despite that seems to be an appropriate name. This is because *Merging* refers to another driver-level optimization which combines subpasses into... something lower level. Refer to [this](https://github.com/KhronosGroup/Vulkan-Samples/tree/main/samples/performance/subpasses#merging) article in the Vulkan samples.


## Some Notable Architectural Choices

I will present some architectural choices in Granite's implementation of the render graph. Again, since I am not the creator of Granite, my resverse-engineered information may not reflect the creator's intentions.

### Input-Output Pairing and Submission Order

Granite implementation specifically enforces that each output must have an input (could be a nullptr (placeholder)), otherwise an error is thrown. The API looks like this:

```c++  
pass.add_color_output("something-comes-out", info, "something-goes-in");  
```  

This would allocate 2 different *logical resources* from the render graph due to the different naming (Granite's render graph relies on resource names to distinguish between unique logical resources, a little "magical" to my taste). But then, when allocating physical resources, resources with this kind of explicit pairing would be mapped to the same physical resource:

```c++  
pass.get_color_outputs()[i]->set_physical_index(input->get_physical_index());  
```  

At first, I didn't quite understand the intention behind it - we could just assign a Read-Write flag to the resource, rather than using some sort of pairing mechanism. Also, the implementation does not prevent naming the input and output as the same thing, which faintly suggests that it is possible to do so - yet after further inspection, it should have been prevented. (Common source of headache in reverse engineering, a slight implementation detail that could lead to ambiguity might make you question whether that is intended).

So let me explain this design choice in a way that I can understand it.

The basic assumption behind it is that the render graph is allowed to rearrange passes - programmers can make suboptimal enqueue calls, and the render graph is tasked to make it optimal. This means we cannot trust the order of submission entirely to resolve the dependency problem. Imagine a submission sequence as follows, assuming we are reasoning with physical textures:

- Pass 1: Reads from GBuffer1, Writes to GBuffer1
- Pass 2: Writes to GBuffer1

If we fully trust the submission order, then we know that GBuffer1 is persistent across frames, Pass 1 would read from the result of Pass 2.

But if render graph can rearrange passes for optimal results, then we would certainly put Pass 2 before Pass 1, because that would actually make GBuffer1 *transient* and remove the need of LoadOp and StoreOp, similar to how we make GBuffer pass and Deferred Lighting pass as 2 sub-passes of a single render pass. (We assume render graph is greedy, always tries to rearrange passes for best performance, the real situation may or may not be the case).

The question is, what if we actually want to maintain the order? What if, for some strange reason, we need to make that GBuffer be accessed as the submission order?

The pairing mechanism effectively allows us to clarify our intention by reasoning with logical resources:

- Pass 1: Reads from GBuffer1, Writes to TextureA
- Pass 2: Writes to TextureA

This may seem strange, as both passes are trying to write to TextureA, and is unclear which one should be executed first. However, a greedy render graph now has no reason to put Pass 2 before Pass 1.

Now, my impression of this design is that it doesn't reduce the burden of the programmer, as if I could submit passes in arbitrary order and the render graph magically rearranges it to make it optimal. The programmer still needs to know an optimal submission order or else they would fail to come up with a meaningful name for the input and outputs. Also, the added layer of render graph being able to reorder the passes makes it inherently more difficult to debug, as programmers can no longer trust their submission order and must draw the graph using the input and output names, and understand how the reordering works under the hood.

What I would instead, is that render graph strictly follows the submission order, no pass can go before another pass submitted before that pass? This makes the render graph more predictable and easier to debug. We could still keep the rearrangement design, but that would become a "diagnostic module", outputting suggestions (print to the console) rather than actually applying it. A programmer must enable the diagnostic module and change the submission order manually if they were to accept the suggestions.

An added benefit of sticking strictly to the submission order is that we can now force a pass to be executed even if they do not contribute to the swapchain image, and the render graph doesn't need to worry where to put that pass. This is useful for cases where, Camera 1 writes to an external texture in Pass A, and that texture is used by a material that is rendered by a UI camera later (Normally, Pass A would be pruned since it does not contribute to Camera 1's frame buffer).