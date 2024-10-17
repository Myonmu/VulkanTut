## Overview

TL;DR: The bumpy ride of Vulkan Learning.

The *classic* tutorial, named Vulkan Tutorial is a semi-great place to start coding in Vulkan, but it comes with a horrific code structure: *everything in one file*.

Hence, the repo tries to mitigate this:

- Vulkan objects will be encapsulated in custom wrappers, and usually they have implicit conversion toward the raw Vulkan object. The wrappers follow RAII.
- Long methods are chopped into smaller chunks
- Better interdependencies between objects

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

