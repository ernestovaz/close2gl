#pragma once

#include "TextureSampler.h"

class TextureSamplerNearest : public TextureSampler {
public:
    TextureSamplerNearest(unsigned char* data, int width, int height);
    vec3 getColor(vec2 uv) override;
};