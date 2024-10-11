//
// Created by miska on 2024/09/02.
//

#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D() = default;

Texture2D::Texture2D(const char *path) {
    pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
}

Texture2D::~Texture2D() {
    stbi_image_free(pixels);
}

int Texture2D::getHeight() const {
    return height;
}

int Texture2D::getWidth() const {
    return width;
}

int Texture2D::getChannelCount() const {
    return channels;
}

VkFormat Texture2D::getFormat() const {
    return VK_FORMAT_R8G8B8A8_SRGB;
}

