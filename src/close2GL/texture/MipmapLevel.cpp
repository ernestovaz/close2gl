#include "MipmapLevel.h"

#include <iostream>
using namespace std;

MipmapLevel::MipmapLevel(int width, int height) 
: width(width), height(height), image(width, vector<vec3>(height, vec3(0.0f))) {}

MipmapLevel::MipmapLevel(unsigned char* texture, int width, int height)
: MipmapLevel(width, height) {
    for(int x = 0; x < image.size(); x++) {
        for(int y = 0; y < image[x].size(); y++) {
            vec3 textureColor = colorFromTexture(texture, width, height, x, y);
            image[x][y] = textureColor;
        }
    }
}

vec3 MipmapLevel::at(int x, int y) {
    return image[x][y];
}

void MipmapLevel::set(int x, int y, vec3 color) {
   image[x][y] = color; 
}

vec3 MipmapLevel::colorFromTexture(unsigned char* texture, int width, int height, int x, int y) {
    vec3 color;
    color.x = texture[(width * y + x) * 3    ];
    color.y = texture[(width * y + x) * 3 + 1];
    color.z = texture[(width * y + x) * 3 + 2];
    return color;
}