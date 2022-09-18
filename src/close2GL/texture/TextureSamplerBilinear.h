#pragma once

#include "TextureSampler.h"

class TextureSamplerBilinear : public TextureSampler {
public:
    TextureSamplerBilinear(unsigned char* data, int width, int height);
    vec3 getColor(vec2 uv) override;
};