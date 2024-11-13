//
// Created by Tereza on 11/1/2024.
//

#include "UnifiedTexture2D.h"

UnifiedTexture2D::UnifiedTexture2D(DeviceContext& ctx, const std::string& path) {
    cpuTexture = std::make_unique<Texture2D>(path.c_str());
    textureImage = std::make_unique<TextureImage>(ctx, *cpuTexture, false);
    textureImage->transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    textureImage->stage();
    textureImage->transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    imageView = std::make_unique<ImageView>(ctx, *textureImage, cpuTexture->getFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
}

UnifiedTexture2D::UnifiedTexture2D(DeviceContext &ctx, const std::string &path, VkFilter mipMapFilter) {
    cpuTexture = std::make_unique<Texture2D>(path.c_str());
    textureImage = std::make_unique<TextureImage>(ctx, *cpuTexture, true);
    textureImage->transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    textureImage->stage();
    textureImage->generateMipmap(cpuTexture->getMaxMipLevel(), mipMapFilter);
    imageView = std::make_unique<ImageView>(ctx, *textureImage, cpuTexture->getFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
}

