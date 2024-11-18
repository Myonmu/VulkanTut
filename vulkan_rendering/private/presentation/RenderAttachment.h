//
// Created by miska on 2024/11/10.
//

#pragma once
#include <map>
#include <vector>

#include "ContextMacros.h"
#include "TextureImage.h"
#include "ImageView.h"


struct WindowContext;
class SwapChain;

enum class AttachmentType {
    MSAA,
    DEPTH_STENCIL,
    PRESENT,
    TRANSIENT_COLOR
};

class RenderAttachment {
public:
    virtual ~RenderAttachment() = default;

    virtual VkAttachmentDescription getAttachmentDescription() const = 0;

    virtual AttachmentType getAttachmentType() const = 0;
};

struct AttachmentRef {
    uint32_t index;
    AttachmentType type;
    VkAttachmentDescription description;
    VkImageLayout layout;

    AttachmentRef() = default;
    AttachmentRef(uint32_t id, const RenderAttachment& attachment, VkImageLayout layout);
};

class ColorAttachment : public RenderAttachment {
    const WindowContext &ctx;
    AttachmentType type;
    VkFormat format{};
    VkSampleCountFlagBits msaaSamples{};
    CTX_PROPERTY(TextureImage, image)
    CTX_PROPERTY(ImageView, imageView)

private:
    void create();

public:
    ColorAttachment(const WindowContext &ctx, VkSampleCountFlagBits msaaSamples, AttachmentType type);

    void recreate();

    VkAttachmentDescription getAttachmentDescription() const override;

    AttachmentType getAttachmentType() const override { return type; };
    operator VkImage() const { return *image; }
    operator VkImageView() const { return *imageView; }
};


class PresentColorAttachment : public RenderAttachment {
    const WindowContext &ctx;
    AttachmentType type = AttachmentType::PRESENT;
    VkFormat format{};
    std::vector<VkImage> swapChainImages;
    std::vector<std::unique_ptr<ImageView> > swapChainImageViews;

    void create();

public:
    ImageView &get_imageView_at(uint32_t i) const;

    explicit PresentColorAttachment(const WindowContext &ctx);

    size_t getSize() const;

    ~PresentColorAttachment() = default;

    VkFormat get_format() const;

    VkAttachmentDescription getAttachmentDescription() const override;

    AttachmentType getAttachmentType() const override { return type; };

    void recreate();
};


class DepthAttachment : public RenderAttachment {
    const WindowContext &ctx;
    AttachmentType type = AttachmentType::DEPTH_STENCIL;
    VkFormat format;
    VkSampleCountFlagBits msaaSamples;
    std::unique_ptr<TextureImage> depthImage;
    std::unique_ptr<ImageView> depthImageView;

    VkFormat findDepthFormat(std::vector<VkFormat> candidates) const;

    static bool hasStencilComponent(const VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void create();

public:
    explicit DepthAttachment(const WindowContext &ctx);

    DepthAttachment(WindowContext &ctx, VkFormat depthFormat, VkSampleCountFlagBits msaaSamples);

    VkAttachmentDescription getAttachmentDescription() const override;

    [[nodiscard]] VkFormat get_format() const;

    void recreate();

    [[nodiscard]] AttachmentType getAttachmentType() const override { return type; };

    operator VkImageView() const { return *depthImageView; }
};
