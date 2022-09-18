#pragma once

#include "TextureSampler.h"

class TextureSamplerTrilinear : public TextureSampler {
public:
    TextureSamplerTrilinear(vector<MipmapLevel> mipmap, int width, int height);
    vec3 getColor(vec2 uv) override;

private:
    vector<MipmapLevel> mipmap;
};