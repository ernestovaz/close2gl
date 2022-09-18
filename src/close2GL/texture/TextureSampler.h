#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using glm::vec2;
using glm::vec3;

class TextureSampler{
public:
    TextureSampler(unsigned char* data, int width, int height);
    virtual vec3 getColor(vec2 uv){ return vec3(0.0f); }
protected:
    vec3 getTexelColor(vec2 coordinate);
    vec3 linearInterpolate(vec3 c1, vec3 c2, float d1, float d2);
    unsigned char* data;
    int width;
    int height;
};