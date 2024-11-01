//
// Created by Tereza on 11/1/2024.
//

#pragma once
#include <Texture2D.h>
#include <TextureImage.h>

#include "ContextMacros.h"
#include "ImageView.h"
#include "ObjectHierarchy.h"

class UnifiedTexture2D : public ObjectNode {
    CTX_PROPERTY(Texture2D, cpuTexture)
    CTX_PROPERTY(TextureImage, textureImage)
    CTX_PROPERTY(ImageView, imageView)
public:
    explicit UnifiedTexture2D(DeviceContext& ctx, const std::string& path);
    ~UnifiedTexture2D() override = default;
};
