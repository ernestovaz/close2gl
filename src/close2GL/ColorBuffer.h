#pragma once

#include <glm/vec3.hpp>

using glm::vec3;

class ColorBuffer {
public:
    ColorBuffer(int width, int height);
    ColorBuffer();
    ~ColorBuffer();
    
    vec3 get(int x, int y);
    void set(int x, int y, vec3 color);
    void resize(int x, int y);
    unsigned char* data();

    int width;
    int height;

private:
    unsigned char* buffer;
};