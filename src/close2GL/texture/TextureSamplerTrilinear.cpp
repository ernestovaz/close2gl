#include "TextureSamplerTrilinear.h"

#include <iostream>
using namespace std;

TextureSamplerTrilinear::TextureSamplerTrilinear(vector<MipmapLevel> mipmap, int width, int height)
: TextureSampler(width, height), mipmap(mipmap) {}

vec3 TextureSamplerTrilinear::getColor(vec2 uv){
    if(uv.x > 1 || uv.x < 0 || uv.y > 1 || uv.y < 0 || uv.x != uv.x || uv.y != uv.y)
        return vec3(0.0f);

    int x = width * uv.x;
    int y = height * uv.y;
    return mipmap[4].at(x/16,y/16);
}