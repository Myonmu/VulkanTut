//
// Created by miska on 2024/09/02.
//

#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <vulkan/vulkan_core.h>

class Texture2D {
public:
    void* pixels = nullptr;
    Texture2D();
    explicit Texture2D(const char* path);
    ~Texture2D();

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getChannelCount() const;
    [[nodiscard]] VkFormat getFormat() const;

private:
    int width = 0, height = 0, channels = 0;
};



#endif //TEXTURE2D_H
