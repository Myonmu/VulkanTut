//
// Created by miska on 2024/11/10.
//

#pragma once
#include <vector>
#include "ImageView.h"


struct WindowContext;
class SwapChain;

class ColorAttachment {
    const WindowContext& ctx;
    VkFormat format{};
    std::vector<VkImage> swapChainImages;
    std::vector<std::unique_ptr<ImageView>> swapChainImageViews;
    void create();
public:
    ImageView& get_imageView_at(uint32_t i) const;
    explicit ColorAttachment(const WindowContext& ctx);
    size_t getSize() const;
    ~ColorAttachment() = default;

    VkFormat get_format() const;
    VkAttachmentDescription getAttachmentDescription() const;
    void recreate();
};
