#include "DepthBuffer.h"

#include <cstdlib>
#include <climits>

DepthBuffer::DepthBuffer() {
    width = 0;
    height = 0;
    buffer = NULL;
}

DepthBuffer::~DepthBuffer() {
    if (buffer != NULL) delete [] buffer;
}

bool DepthBuffer::update(int x, int y, float depth) {
    int position = (width * y + x);
    if(depth < buffer[position]){
        buffer[position] = depth;
        return true;
    } 
    else return false;
}

void DepthBuffer::clear() {
    for(int i = 0; i+3 < width*height; i++) {
        buffer[i] = INT_MAX;
    }
}

void DepthBuffer::resize(int width, int height){ 
    if (buffer != NULL) delete [] buffer;
    this->width = width;
    this->height = height;
    buffer = new float[width * height];  
}