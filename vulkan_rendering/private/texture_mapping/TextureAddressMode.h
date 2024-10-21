//
// Created by miska on 10/10/2024.
//

#pragma once
#include "TextureFilterMode.h"


struct TextureAddressMode {

  VkSamplerAddressMode u{};
  VkSamplerAddressMode v{};
  VkSamplerAddressMode w{};

  const static TextureAddressMode REPEAT;
  const static TextureAddressMode CLAMP_TO_EDGE;
  const static TextureAddressMode CLAMP_TO_BORDER;
  const static TextureAddressMode MIRRORED_REPEAT;
  const static TextureAddressMode MIRROR_CLAMP_TO_EDGE;
};


