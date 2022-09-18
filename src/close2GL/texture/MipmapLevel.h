#pragma once

#include <vector>
#include <glm/vec3.hpp>

using std::vector;
using glm::vec3;

class MipmapLevel {
public:
    int width;
    int height;

    MipmapLevel(int width, int height);
    MipmapLevel(unsigned char* image, int width, int height);

    vec3 at(int x, int y);
    void set(int x, int y, vec3 color);

private:
    vector<vector<vec3>> image;

    static vec3 colorFromTexture(unsigned char* texture, int width, int height, int x, int y);
};