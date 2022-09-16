#include "TextureSamplerNearest.h"

TextureSamplerNearest::TextureSamplerNearest(unsigned char* data, int width, int height)
: TextureSampler(data, width, height) {}

vec3 TextureSamplerNearest::getColor(vec2 uv){
    vec3 color;

    int x = uv.x * width;
    int y = uv.y * height;

    color.x = data[(height * x + y) * 3 + 0];
    color.y = data[(height * x + y) * 3 + 1];
    color.z = data[(height * x + y) * 3 + 2];

    return color;
}