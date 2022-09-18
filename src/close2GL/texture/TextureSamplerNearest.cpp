#include "TextureSamplerNearest.h"


TextureSamplerNearest::TextureSamplerNearest(unsigned char* data, int width, int height)
: TextureSampler(data, width, height) {}

vec3 TextureSamplerNearest::getColor(vec2 uv){
    vec3 color;

    int x = uv.x * width;
    int y = uv.y * height;

    return getTexelColor(x,y);
}