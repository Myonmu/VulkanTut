## Overview

TL;DR: The bumpy ride of Vulkan Learning.

The *classic* tutorial, named Vulkan Tutorial is a semi-great place to start coding in Vulkan, but it comes with a horrific code structure: *everything in one file*.

Hence, the repo tries to mitigate this:

- Vulkan objects will be encapsulated in a custom wrapper, and usually they have implicit conversion towards the raw Vulkan object. The wrappers follow 
- Long methods are chopped into smaller chunks

## Notes

- Objects that persist through the app's lifetime are inside `VulkanAppContext`.
- To record commands on CommandBuffer, first allocate a command buffer, and then create a `CommandBufferRecorder`, call `enqueueCommand` to enqueue commands on the recorder, finally call `recordCommandBuffer` to flush the command on the command buffer.
-  
