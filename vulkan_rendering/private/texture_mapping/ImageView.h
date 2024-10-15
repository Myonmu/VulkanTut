//
// Created by miska on 10/10/2024.
//

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>


struct DeviceContext;

class ImageView : public VulkanResource<VkImageView, DeviceContext>{
public:
    ImageView(DeviceContext& ctx, const VkImage &image, VkFormat format);
    ~ImageView() override;
};



#endif //IMAGEVIEW_H
