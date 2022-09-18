#include "TextureSampler.h"

TextureSampler::TextureSampler(unsigned char* data, int width, int height) {
    this->data = data;
    this->width = width;
    this->height = height;
}

vec3 TextureSampler::getTexelColor(vec2 coordinate) {
    if(coordinate.x >= width || coordinate.x < 0 || coordinate.y >= height || coordinate.y < 0) 
        return vec3(0.0f);

    vec3 color;
    color.x = data[(width * (int)coordinate.y + (int)coordinate.x) * 3];
    color.y = data[(width * (int)coordinate.y + (int)coordinate.x) * 3 + 1];
    color.z = data[(width * (int)coordinate.y + (int)coordinate.x) * 3 + 2];
    return color;
}

vec3 TextureSampler::linearInterpolate(vec3 c1, vec3 c2, float d1, float d2){
    return (c1 * d2 + c2 * d1) / (d1 + d2);
}