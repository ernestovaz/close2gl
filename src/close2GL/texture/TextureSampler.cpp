#include "TextureSampler.h"

TextureSampler::TextureSampler(unsigned char* data, int width, int height) {
    this->data = data;
    this->width = width;
    this->height = height;
}

vec3 TextureSampler::getTexelColor(int x, int y) {
    vec3 color;
    color.x = data[(height * x + y) * 3];
    color.y = data[(height * x + y) * 3 + 1];
    color.z = data[(height * x + y) * 3 + 2];
    return color;
}