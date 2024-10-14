//
// Created by miska on 2024/10/10.
//

#ifndef TEXTUREANISOTROPYINFO_H
#define TEXTUREANISOTROPYINFO_H
#include <optional>
#include <vulkan/vulkan_core.h>


struct DeviceContext;
struct VulkanAppContext;

struct TextureAnisotropyInfo {
    bool enabled{VK_TRUE};
    float maxAnisotropy{1.0f};

    static TextureAnisotropyInfo getAutoAnisotropyInfo();
    static bool isAnisotropySupported();
    static float getMaxAnisotropy();
    static void queryAnisotropyInfo(const DeviceContext& ctx);
private:
    static std::optional<bool> supportsAnisotropy;
    static std::optional<float> supportedMaxAnisotropy;

};



#endif //TEXTUREANISOTROPYINFO_H
