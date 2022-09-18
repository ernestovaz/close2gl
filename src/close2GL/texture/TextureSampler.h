#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "MipmapLevel.h"

using std::vector;
using glm::vec2;
using glm::vec3;

class TextureSampler{
public:
    TextureSampler(int width, int height);
    TextureSampler(unsigned char* data, int width, int height);
    virtual vec3 getColor(vec2 uv){ return vec3(0.0f); }

    static vector<MipmapLevel> generateMipmap(unsigned char* data, int width, int height);

protected:
    unsigned char* data;
    int width;
    int height;

    vec3 getTexelColor(vec2 coordinate);
    vec3 linearInterpolate(vec3 c1, vec3 c2, float d1, float d2);

    static vec3 averageColor(vector<vec3> colors);
};