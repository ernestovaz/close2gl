#include "TextureSamplerNearest.h"


TextureSamplerNearest::TextureSamplerNearest(unsigned char* data, int width, int height)
: TextureSampler(data, width, height) {}

vec3 TextureSamplerNearest::getColor(vec2 uv){
    int x = uv.x * width;
    int y = uv.y * height;
    vec2 coordinate(x,y);
    return getTexelColor(coordinate);
}