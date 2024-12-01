//
// Created by miska on 2024/11/10.
//

#pragma once
#include <map>
#include <vector>

#include "ContextMacros.h"
#include "TextureImage.h"
#include "ImageView.h"
#include "Polymorphism.h"


struct AttachmentInfo;
struct WindowContext;
class SwapChain;

enum class AttachmentType {
    DEPTH_STENCIL,
    PRESENT,
    COLOR
};

struct AttachmentInfo : public TextureImageInfo {
    uint32_t index{};
    AttachmentType type{};
    VkAttachmentDescription description{};
    VkImageLayout layout{};
    VkClearColorValue clearColor{};

    AttachmentInfo();

    AttachmentInfo(uint32_t w, uint32_t h, uint32_t channels, VkFormat format, VkImageUsageFlags usage)
        : TextureImageInfo(w, h, channels, format, usage) {
    };

    AttachmentInfo(TexturePxDimensions &dimensions, uint32_t channels, VkFormat format, VkImageUsageFlags usage)
        : TextureImageInfo(dimensions, channels, format, usage) {
    };
};

class RenderAttachment {
public:
    std::string name;

    virtual ~RenderAttachment() = default;

    [[nodiscard]] virtual VkAttachmentDescription getAttachmentDescription() const = 0;

    [[nodiscard]] virtual AttachmentType getAttachmentType() const = 0;

    bool isCompatibleWith(AttachmentInfo &ref);
};


class ColorAttachment : public RenderAttachment {
    const WindowContext &ctx;
    AttachmentType type;
    VkFormat format{};
    CTX_PROPERTY(TextureImage, image)
    CTX_PROPERTY(ImageView, imageView)

private:
    void create();

public:
    ColorAttachment(const WindowContext &ctx);

    void recreate();

    [[nodiscard]] VkAttachmentDescription getAttachmentDescription() const override;

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
    [[nodiscard]] ImageView &get_imageView_at(uint32_t i) const;

    explicit PresentColorAttachment(const WindowContext &ctx);

    [[nodiscard]] size_t getSize() const;

    ~PresentColorAttachment() override = default;

    [[nodiscard]] VkFormat get_format() const;

    [[nodiscard]] VkAttachmentDescription getAttachmentDescription() const override;

    [[nodiscard]] AttachmentType getAttachmentType() const override { return type; };

    void recreate();
};


class DepthAttachment : public RenderAttachment {
    const WindowContext &ctx;
    AttachmentInfo info{};
    AttachmentType type = AttachmentType::DEPTH_STENCIL;
    std::unique_ptr<TextureImage> depthImage;
    std::unique_ptr<ImageView> depthImageView;

    [[nodiscard]] VkFormat findDepthFormat(std::vector<VkFormat> candidates) const;

    static bool hasStencilComponent(const VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void create();

public:
    explicit DepthAttachment(const WindowContext &ctx);

    DepthAttachment(const WindowContext &ctx, AttachmentInfo &info);

    [[nodiscard]] VkAttachmentDescription getAttachmentDescription() const override;

    [[nodiscard]] VkFormat get_format() const;

    void recreate();

    [[nodiscard]] AttachmentType getAttachmentType() const override { return type; };

    operator VkImageView() const { return *depthImageView; }
};


/**
 * Centralized attachment manager.
 */
class AttachmentManager {
    WindowContext &ctx;
    poly_vector<RenderAttachment> attachments;

public:
    void recreate();

    RenderAttachment &getOrCreateAttachment(const AttachmentInfo &attachmentRef);

    explicit AttachmentManager(WindowContext &ctx);
};
