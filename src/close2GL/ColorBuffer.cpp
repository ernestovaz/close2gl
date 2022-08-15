#include "ColorBuffer.h"

ColorBuffer::ColorBuffer() {
    width = 0;
    height = 0;
    buffer = NULL;
};

ColorBuffer::ColorBuffer(int width, int height) {
    this->width = width;
    this->height = height;
    buffer = new unsigned char[width * height * 3];  
}

ColorBuffer::~ColorBuffer() {
    delete [] buffer;
}

vec3 ColorBuffer::get(int x, int y) {
    vec3 color;
    color.x = buffer [(height * x + y) * 3];
    color.y = buffer [(height * x + y) * 3 + 1];
    color.z = buffer [(height * x + y) * 3 + 2];
    return color;
}

void ColorBuffer::set(int x, int y, vec3 color) {
    buffer [(width * y + x) * 3]      = (unsigned char) color.x;
    buffer [(width * y + x) * 3 + 1]  = (unsigned char) color.y;
    buffer [(width * y + x) * 3 + 2]  = (unsigned char) color.z;
}

void ColorBuffer::resize(int width, int height) {
    if (buffer != NULL) delete [] buffer;
    this->width = width;
    this->height = height;
    buffer = new unsigned char[width * height * 3];  
}

unsigned char* ColorBuffer::data() {
    return buffer;
}
 