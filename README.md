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
