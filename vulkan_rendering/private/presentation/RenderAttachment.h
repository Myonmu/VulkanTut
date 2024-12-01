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

class RenderAttachment {
public:
    std::string name;

    virtual ~RenderAttachment() = default;

    [[nodiscard]] virtual VkAttachmentDescription getAttachmentDescription() const = 0;

    [[nodiscard]] virtual AttachmentType getAttachmentType() const = 0;

    bool isCompatibleWith(AttachmentInfo& ref);
};

struct AttachmentInfo: public TextureImageInfo {
    uint32_t index{};
    AttachmentType type{};
    VkAttachmentDescription description{};
    VkImageLayout layout{};
    VkClearColorValue clearColor{};
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
    AttachmentType type = AttachmentType::DEPTH_STENCIL;
    VkFormat format;
    VkSampleCountFlagBits msaaSamples;
    std::unique_ptr<TextureImage> depthImage;
    std::unique_ptr<ImageView> depthImageView;

    [[nodiscard]] VkFormat findDepthFormat(std::vector<VkFormat> candidates) const;

    static bool hasStencilComponent(const VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void create();

public:
    explicit DepthAttachment(const WindowContext &ctx);

    DepthAttachment(WindowContext &ctx, VkFormat depthFormat, VkSampleCountFlagBits msaaSamples);

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
    WindowContext& ctx;
    poly_vector<RenderAttachment> attachments;
public:
    void recreate();
    RenderAttachment& getOrCreateAttachment(const AttachmentInfo& attachmentRef);
    explicit AttachmentManager(WindowContext &ctx);
};