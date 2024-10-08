//
// Created by miska on 2024/09/02.
//

#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class Texture2D {
public:
    Texture2D();
    explicit Texture2D(const char* path);
    ~Texture2D();

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getChannelCount() const;
private:
    stbi_uc* pixels;
    int width = 0, height = 0, channels = 0;
};



#endif //TEXTURE2D_H
