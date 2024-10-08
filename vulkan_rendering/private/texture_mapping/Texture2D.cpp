//
// Created by miska on 2024/09/02.
//

#include "Texture2D.h"

Texture2D::Texture2D() {

}

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


