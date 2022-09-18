#include "TextureSampler.h"

#include <cmath>
#include <cstdlib>

#include <iostream>
using namespace std;

using std::max;

TextureSampler::TextureSampler(int width, int height) {
    this->data = NULL;
    this->width = width;
    this->height = height;
}

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

vector<MipmapLevel> TextureSampler::generateMipmap(unsigned char* data, int width, int height){
    vector<MipmapLevel> mipmaps;
    MipmapLevel firstLevel(data, width, height);
    mipmaps.push_back(firstLevel);
    
    int currentWidth = width/2;
    int currentHeight = height/2;

    while(currentWidth > 1 && currentHeight > 1){
        MipmapLevel current(currentWidth, currentHeight); 
        MipmapLevel previous = mipmaps.back();
        for(int x = 0; x < current.width; x++){
            for(int y = 0; y < current.height; y++){

                vec2 scaled(x * 2, y * 2);

                vector<vec3> colors;
                colors.reserve(4);
                colors.push_back(previous.at(scaled.x, scaled.y));
                if(scaled.x+1 < previous.width) 
                    colors.push_back(previous.at(scaled.x+1, scaled.y));
                if(scaled.y+1 < previous.height) 
                    colors.push_back(previous.at(scaled.x, scaled.y+1));
                if(scaled.x+1 < previous.width && scaled.y+1 < previous.height) 
                    colors.push_back(previous.at(scaled.x+1, scaled.y+1));

                vec3 average = averageColor(colors);
                current.set(x, y, average);
            }
        }
        currentHeight/=2;
        currentWidth/=2;

        mipmaps.push_back(current);
    }
    return mipmaps;
}

vec3 TextureSampler::averageColor(vector<vec3> colors){
    vec3 sum;
    long redSum = 0, blueSum = 0, greenSum = 0; 
    for(vec3 color : colors){
        redSum += color.x;
        blueSum += color.y;
        greenSum += color.z;
    }
    long count = colors.size();
    return vec3(redSum/count, blueSum/count, greenSum/count);
}