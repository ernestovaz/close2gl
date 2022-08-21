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
    if (buffer != NULL) delete [] buffer;
}

vec3 ColorBuffer::get(int x, int y) {
    vec3 color;
    color.x = buffer [(height * x + y) * 3];
    color.y = buffer [(height * x + y) * 3 + 1];
    color.z = buffer [(height * x + y) * 3 + 2];
    return color;
}

void ColorBuffer::setColor(int x, int y, vec3 color) {
    int position = (width * y + x) * 3;
    buffer [position    ]  = (unsigned char) color.x;
    buffer [position + 1]  = (unsigned char) color.y;
    buffer [position + 2]  = (unsigned char) color.z;
}

void ColorBuffer::clear(vec3 color) {
    unsigned char colorR = color.x;
    unsigned char colorG = color.y;
    unsigned char colorB = color.z;
    
    for(int i = 0; i+3 < width*height*3; i+=3) {
        buffer[i] =     colorR;
        buffer[i+1] =   colorG;
        buffer[i+2] =   colorB;
    }
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
 