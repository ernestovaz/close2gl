#include "TextureSampler.h"

TextureSampler::TextureSampler(unsigned char* data, int width, int height) {
    this->data = data;
    this->width = width;
    this->height = height;
}