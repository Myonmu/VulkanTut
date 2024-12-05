# Render Pass Resource Management

*This was originally 2 separate articles, due to their relevance, are now merged.*

## VkSwapchainKHR, VkRenderPass, and VkFrameBuffer

`VkSwapchainKHR` is effectively per-window, and in the tutorial, `VkRenderPass` is created with a swapchain's image format, and `VkFrameBuffer` requires both a swapchain's image attachments and be bound to **a single render pass**. And this would create a strong dependency chain on our first sight, though upon closer inspection, not necessarily.

In the tutorial, `VkRenderPass` only requires an image format, but it doesn't state that the image format should come from a swapchain. (Well, actually yes, you *would* use the swapchain's image format, but you don't need to access the swapchain during render pass creation.) Thus instead of accessing swapchain's member field, using a constructor  
Injection would decouple the strong relationship between swapchain and render pass. (Note: constructor injection is just a fancy way to say pass the image format as constructor argument)

This means that multiple swapchains can effectively use the same render pass if their image format and other bits are compatible.

Actually, that's not quite right. since swapchains do not *use* a render pass directly. This relationship comes with `VkFrameBuffer` as it uses both a swapchain and a render pass - we ensure that the swapchain and the render pass are compatible when creating frame buffers, or else we first need to create a compatible render pass. By "compatible, " it isn't just the image format, but also store/load operations, etc.

```  
VkSwapchainKHR  
      \1            VkRenderPass  
       \            /1  
        \*         /*  
        VkFrameBuffer   
```  

The compatibility requirements seem strict, but consider the following scenario: a scene is being rendered with multiple windows, using the same device and likely the same render pipeline. For consistency, there's no reason to fiddle with the render pass parameters, thus, it is likely that we can use the same render pass for all these windows.

*Multi-window rendering reusing the same render pass:*  
![image](https://github.com/user-attachments/assets/26518b13-2121-4658-9564-cda472b0c583)

### Render Pass and Associated Resources

The original tutorial has only one render pass with only one subpass. The detailed breakdown of these different components is detailed below in the Render Graph section. Here I would only outline the top-level relationships between Render Pass related concepts.

For each window, we have only one swapchain. This means we can only have a single color output that gets rendered to that window (surface, to be exact). Because of frame overlaps, we have multiple swapchain images, and during each render loop, we acquire one from the swapchain. Now, these swapchian-specific VkImage(s) are created differently, by calling `vkGetSwapchainImagesKHR` (see `PresentColorAttachment`). We then need to write to this specific image during render loop to actually show something on the window.

To use this swapchain image, two things need to be done:

- Reference it in a frame buffer.
- Use the frame buffer in a render pass.

The swapchain image is like any other color attachments, except for being used to present the result. Frame buffer is per-Render Pass, so it means when we have multiple render passes, we don't necessarily write to a swapchain image, it all depends on the declaration of the frame buffer and whether the image is passed to color attachment during render pass and subpass creation.

Well, render pass is created earlier than frame buffers (because we need a render pass reference in frame buffer create info), so logically we create frame buffers based on the declared usages in a render pass. This is an important concept that we use in render graph.

As for subpasses, they are inside render pass and use a subset of the frame buffers. The details are in the Render Graph section.

```  
  Window  
    \                                 RenderPass 1------* Subpass  
     \                                    |           
  Swapchain   ColorAttachments 0*---1 FrameBuffer  
       \1                            /     /  
        \                         01/     /  
         \     DepthStencilAttachment    /  
          \*                            /  
          PresentColorAttachment 01----/  
```  

Render Pass does not directly reference an attachment's handle, but only through *Attachment Description* and *Attachment Reference*, which can exist before the attachment is even created. The actual correspondence is done through the frame buffer (or descriptor sets if not managed by renderpass).

