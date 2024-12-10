# Attachment Referencing

The Render Pass system has a rather complex attachment referencing mechanism, and one little misalignment will result in painful bug hunting (one more reason to prefer render graph to ensure proper referencing).

There are several orders that are important:

In shader, we have input attachment declared:
```glsl  
layout (input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput inputPosition;  
```  
This means we expect an input attachment at 0, and the same `VkImageView` is bound at binding 0 of descriptor set 1. Now, "input attachment at 0" is a vague description. What is this "0" exactly?

Turns out it is *the index of the attachment reference in Subpass description* - not the index of the attachment in frame buffer:

```c++  
VkSubpassDescription Subpass::getSubpassDescription() const {  
    return {  
         // other fields are not shown   
        .inputAttachmentCount = static_cast<uint32_t>(inputAttachments.size()),  
        .pInputAttachments = inputAttachments.data(), //<- corresponds to index in this vector/array  
    };  
}  
```  

Even if the attachment reference is `{.attachment = 2}`, if it is the first in the attachment reference array, in shader, `input_attachment_index` should be 0.

Now, the `.attachment` in `VkAttachmentReference` points to the index of the attachment in Frame Buffer ... if configured correctly. It actually points to the `.pAttachments` array index in `VkRenderPassCreateInfo`

```c++  
    VkRenderPassCreateInfo renderPassInfo{};  
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;  
    renderPassInfo.attachmentCount = static_cast<uint32_t>(descriptions.size());  
    renderPassInfo.pAttachments = descriptions.data();  
    //...  
```  

The order of the `.pAttachments` should be used to create the frame buffer, or else you get a mismatch:

```c++  
    VkFramebufferCreateInfo framebufferInfo{};  
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;  
    framebufferInfo.renderPass = renderPass;  
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());  
    framebufferInfo.pAttachments = attachments.data(); //<- order should be the same as in render pass  
    //...  
```  

Since inputs are connected to some outputs, which means the order of the outputs must also follow the next input. This is again specified in Subpass:

```c++  
VkSubpassDescription Subpass::getSubpassDescription() const {  
    return {  
         // other fields are not shown   
        .colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size()),  
        .pColorAttachments = colorAttachments.data(),  
    };  
}  
```  

Finally, the `clearColors` in `VkRenderPassBeginInfo` should also follow the declaration order in `VkRenderPass`, and by extension, the order in frame buffer. If you accidentally give a color texture a depth clear value, then you will see weird things.

![schema: attachment referencing](https://github.com/user-attachments/assets/3304548c-f0ae-4503-9013-d2e9e4b13253)
