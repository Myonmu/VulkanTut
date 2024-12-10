# Overview of Unreal's Render Dependency Graph (RDG)

Unreal's implementation of Render Graph, or RDG, is nothing like other implementations that I have seen.

In this section, I will portrait a bird's-eye view of the RDG. It is only useful if you plan on making a render graph system.

Before I dive deeper into the code, Unreal was originally coded for D3D, and that means a lot of the concepts are higher-level abstractions and do not really exist as a single concept in Vulkan. Here are some terms that I came across:

- UAV (Unordered Access View): It is a resource that allows random read/write. This essentially translates to storage textures/buffers with appropriate access flags and layouts.
- RTV (Render Target View): A texture view that serves as render target. It is read-write.
- SRV (Shader Resource View): Textures and buffers that are bound to a shader, read-only. 

## Access Declarations

Unreal has a concept of **Shader Parameter Struct**, a struct that holds all the resources used in a shader, or an RDG pass. Typically, creating an RDG pass in Unreal follows these steps:

- Define a shader parameter struct
- Allocate the struct with `GraphBuilder.AllocParameters`.
- Assign the struct members with resource handles created from the graph builder.
- Call `GraphBuilder.AddPass`, passing the filled struct, to add a pass to the RDG.

The declaration of usage is in the first step: declaring the struct, rather than calling a method later after pass creation (Unity's approach).

Observe this example code from Unreal's documentations:

```c++
class FMyComputeShader: public FGlobalShader
{
	public:
		DECLARE_GLOBAL_SHADER(FMyComputeShader);
		SHADER_USE_PARAMETER_STRUCT(FMyComputeShader, FGlobalShader);
 
		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
 
			// Declares read access to an FRDGTexture* which maps to 'MyTexture' in HLSL code.
			SHADER_PARAMETER_RDG_TEXTURE(Texture2D, MyTexture)
 
			// Declares read access to an FRDGTextureSRV* which maps to 'MySRV' in HLSL code.
			SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D, MySRV)
 
			// Declares write access to an FRDGTextureUAV* which maps to 'MyUAV' in HLSL code.
			SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D, MyUAV)
 
			// A float shader parameter, which maps to 'MyFloat' in HLSL code. Ignored by RDG.
			SHADER_PARAMETER(float, MyFloat)
 
		END_SHADER_PARAMETER_STRUCT()
};
```

The access semantics are very similar to how they are declared in hlsl. A read-only texture is declared simply as `Texture2D`, and a read-write texture is declared as `RWTexture2D`. (Write-only doesn't exist, and can usually be substituted with read-write).

However, Unreal also provides a slightly different, more verbose syntax when no shader is associated with a shader parameter struct:

```c++
BEGIN_SHADER_PARAMETER_STRUCT(FCopyTextureParameters, )
 
	// Declares CopySrc access to an FRDGTexture*
	RDG_TEXTURE_ACCESS(Input,  ERHIAccess::CopySrc)
 
	// Declares CopyDest access to an FRDGTexture*
	RDG_TEXTURE_ACCESS(Output, ERHIAccess::CopyDest)
 
END_SHADER_PARAMETER_STRUCT()
```

The `RHIAccess` enum resembles `VkImageUsageFlags`. The macro expansion is the same, because all the above declarations use `INTERNAL_SHADER_PARAMETER_EXPLICIT` macro under the hood.

In `FRDGBuilder::SetupPassResources`, we can see an `EnumerateTextureAccess` call on pass parameters which prove our previous assumptions. As for how the enumeration is done is outside of our scope, just assume that Unreal uses macro magic to generate static reflection data to play with.

```c++
// the lambda receives reflection data of PassParameters
EnumerateTextureAccess(PassParameters, PassFlags, [&](FRDGViewRef TextureView, FRDGTextureRef Texture, ERHIAccess Access, ERDGTextureAccessFlags AccessFlags, FRDGTextureSubresourceRange Range)  
{
 //...
}
```
This will store the state of the resource in the member array field of the pass `TArray<FTextureState, FRDGArrayAllocator> TextureStates`, and is used later during RDG compilation.

The technical choice behind this implementation is, as stated in the documentation:

	The coupling of pass parameters with shader parameters is intentional. The majority of pass parameters across the Unreal Engine are also shader parameters. Declaring them both using the same API reduces boilerplate.

And this can be seen in the shader binding process (or more precisely in Vulkan lingo, descriptor updates and bindings):

```c++
TShaderMapRef<FMyShaderCS> ComputeShader(View.ShaderMap);
RHICmdList.SetComputeShader(ComputeShader.GetComputeShader());
 
FMyShaderCS::FParameters ShaderParameters;
 
// Assign the parameters.
ShaderParameters.ViewportSize = View.ViewRect.Size();
ShaderParameters.World = 1.0f;
ShaderParameters.FooBarArray[4] = FVector(1.0f, 0.5f, 0.5f);
 
// Submit the parameters.
SetShaderParameters(RHICmdList, ComputeShader, ComputeShader.GetComputeShader(), Parameters);
 
RHICmdList.DispatchComputeShader(GroupCount.X, GroupCount.Y, GroupCount.Z);

```

We can see that we directly assign resources to the parameter struct, and then bind the whole struct to the shader (or pipeline) with a single call. This defers from Unity's approach of needing to call `material.SetTexture("_FieldName", resource)` for each field you want to assign. This "batched" behavior is a clear indicator of batched binding call. Remember that binding calls are actually expensive, and batching would improve performance.

##### Resource Allocation

In render graphs, we usually have 2 different types of allocations:

- **Transient**: The resource is allocated by the render graph system, and access is only guaranteed inside a pass that declares usages on it. In RDG this is called a *Pooled Resource*.
- **External** or **Imported**: These are not managed by the render graph system, and you are responsible for allocating them your self. RDG also supports *Extracting* a resource that was allocated by the render graph to be used outside of the render graph.

In RDG, all these resources are eventually represented by `FRDGResource` or one of its derived types. And this is a how we create one from the graph builder:

```c++
// allocate pass parameters
FMyShaderCS::FParameters* PassParameters = GraphBuilder.AllocParameters<FMyShaderCS::FParameters>();
// create an UAV and assign it to the parameters
// NewSceneColor is an FRDGTextureUAVDesc (like create info in Vulkan)
PassParameters->SceneColorOutput = GraphBuilder.CreateUAV(NewSceneColor);
```

Here, "create" differs from "allocate" - what is being created is just an empty shell, the actual texture is only allocated and filled in this empty shell during the **Compile** phase of the render graph.

A quick revision of Unreal's abstraction on rendering resources, with the added complexity of RDG:

- **RDG resources**: The highest level in the hierarchy. It may not always hold a physical resource, but they hold enough information to allocate one if needed.
- **RHI resources**: An abstraction over the raw, API-specific resource handles. An RDG resource secretly stores an RHI resource handle.
- **API-specific resources**: Derived from `FRHIResource` and hold the actual resource handles.

In fact, RDG resource's type hierarchy mimics closely that of the RHI resource. You would usually find pairs by substituting RDG and RHI in type names. For instance: `FRDGTexture` and `FRHITexture`.

### External Resources

Graph Builder provides `RegisterExternalTexture` methods to register an external texture. The same methods are available for buffers as well. But curiously, you cannot directly provide an RHI handle - the methods expect `TRefCountPtr<IPooledRenderTarget>`. This is commonly obtained from extracting a resource from the render graph. You could, however, convert an RHI handle to make it RDG compatible, by calling `CreateRenderTarget`:

```c++
InstanceData->TransientRDGTexture = Context.GetGraphBuilder().RegisterExternalTexture(CreateRenderTarget(ResolvedTextureRHI, TEXT("NiagaraTexture2DArray")));
```

The Graph Builder ensures that external resource is imported only once. Multiple imports will always result in getting the same RDG handle.

Since we already have the physical resource here, the graph builder will directly link the RDG handle with the RHI handle (`SetPooledRenderTargetRHI` and similar).

### Pooled Resources

"Pooled Resources" are simply created by calling `CreateTexture` or some other appropriate methods, passing an appropriate descriptor struct (similar to Create Info structs in Vulkan). The resource is not yet allocated, only a "shell", as I explained before.

Each resource store a `FirstPass` and a `LastPass` reference, these represent the first pass in render graph where the resource is used, and the last. This information is **filled during the allocation collection steps**. The resource's scope, if not external, is clamped between these two passes. This information will come in handy when deciding resource aliasing (using the same physical resource for two different logical resources, when those two logical resource's scopes do not overlap).

The actual process is tedious and I will only go through the basics here.

RDG calls `CollectAllocations` on each unculled pass, and this will runthrough all the resource states stored in the pass, generating allocation operations.

In `RenderGraphBuilder::Execute()`, before scheduling pooled resource allocations, there is a collection step that determines what should be allocated. The collection step essentially transforms the access information stored in each pass into `FCollectResourceOp`. The struct references the originated pass as well as the resource index.

After collection, the allocation task is scheduled as follows:

```c++
AllocatePooledTexturesTask = AddCommandListSetupTask([this, PooledTextures = MoveTemp(CollectResourceContext.PooledTextures)] (FRHICommandListBase& RHICmdListTask)  
{  
    AllocatePooledTextures(RHICmdListTask, PooledTextures);  
  
}, TaskPriority);
```

## Graph Compilation

Several things happen during the compilation phase. I will skip the less interesting steps and go straight to the juicy parts.

### Pass Dependency Setup

This step bookkeeps for each pass, what are their producers. When we talk about producers of a *pass*, it is essentially by looking at *who produce the resources that I use in this pass*. The producer info is directly stored in the pass (`FRDGPass`):

```c++
/** Handle of the latest cross-pipeline producer. */
FRDGPassHandle CrossPipelineProducer;

//... skip some lines

/** Lists of producer passes and the full list of cross-pipeline consumer passes. */
TArray<FRDGPassHandle, FRDGArrayAllocator> CrossPipelineConsumers;
TArray<FRDGPass*, FRDGArrayAllocator> Producers;
```

Unreal adds extra bookkeeping for cross pipeline producers. By cross pipeline, it means from a separate queue (Graphics vs Async Compute).The `Producers` array is unordered, but `CrossPipelineConsumers` is strictly ordered by submission order (or pass id), keeps track of all the consumers from the other queue. `CrossPipelineProducer` is the producer from another queue with the largest pass id (hence latest).

In `FRDGBuilder::AddPassDependency` which takes a *producer pass* and a *consumer pass*, we can see this bookkeeping in action:

- if *producer* and *consumer* are not in the same pipeline (queue), then the *consumer* is added to the `CrossPipelineConsumers` array. The *producer* becomes *consumer*'s `CrossePipelineProducer` if the *producer* is later than the existing cross pipeline producer. 
- Add the *producer* to the `Producers` array of the *consumer*.

Now the interesting part is that the producer keeps an array of consumers, rather than the earliest. In theory, we only need to sync with the earliest consumer. But this step happens before *Pass Culling*, which means if we simply keep the earliest consumer, it might have been culled and we won't be able to find the next consumer and then we can't properly sync between queues. For the consumer, since it depends on the producer, when the consumer isn't culled, then the producer must be also persist, hence we do not need to keep an array like the producer does. 

## The Graph

Unreal has some unique concepts that are daunting at first.

- Prologue/Epilogue : Unreal uses these two terms frequently inside RDG, and they don't seem to relate to any render graph concept immediately. In a nutshell, *Prologue* means prepare to do something for the following pass, and *Epilogue* means cleaning things up. The RDG itself has one *Prologue Pass* and one *Epilogue Pass*, all created automatically by the RDG builder and are empty. In pass merging, there are also *Prologue Barrier Pass* and *Epilogue Barrier Pass*, these passes are simply extra roles assigned to the first and last passes in a merge, and manages pipeline barriers (because whatever resource transition is required in the middle, must now be done in the first and last passes). Think about top of the pipe and bottom of the pipe execution stages in Vulkan, they essentially do nothing, just convenient places at the start and the end of an execution.
- To Begin/To End : *To Begin* means something that has to be prepared before executing a pass, and *To End* means something that has to be finalized. These can be conveniently translated into pipeline/resource barriers.

This local function in `Compile()` shows how merging causes *To Begin* responsibility shift to the last pass of the merge, with added comments:

```c++
const auto SetEpilogueBarrierPass = [&](FRDGPass* Pass, FRDGPassHandle EpilogueBarrierPassHandle)  
{  
    Pass->EpilogueBarrierPass = EpilogueBarrierPassHandle;  
    // this pass no longer cares what resources to end since it is in the middle
	Pass->ResourcesToEnd.Reset();
	// shifts the responsibility to the epilogue pass (last pass in merge sequence)  
    Passes[EpilogueBarrierPassHandle]->ResourcesToEnd.Add(Pass);  
};
```
## Pass Merging

Pass merging in unreal is similar to how pass merging work in Vulkan: if they share the same set of frame buffers.