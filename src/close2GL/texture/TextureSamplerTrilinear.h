#pragma once

#include "TextureSampler.h"

class TextureSamplerTrilinear : public TextureSampler {
public:
    TextureSamplerTrilinear(vector<MipmapLevel> mipmap, int width, int height);
    vec3 getColor(vec2 uv) override;
    void setScanlineIncrement(vec2 increment) override;

private:
    vector<MipmapLevel> mipmap;
    vec2 increment;
    
    double calculateLevel(vec2 uv);
    vec3 interpolateLevel(int level, vec2 uv);
};