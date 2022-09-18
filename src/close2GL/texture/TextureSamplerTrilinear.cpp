#include "TextureSamplerTrilinear.h"
#include <cmath>
#include <algorithm>

#include <iostream>
using namespace std;

using std::clamp;
using std::max;

TextureSamplerTrilinear::TextureSamplerTrilinear(vector<MipmapLevel> mipmap, int width, int height)
: TextureSampler(width, height), mipmap(mipmap) {}

vec3 TextureSamplerTrilinear::getColor(vec2 uv){
    if(uv.x != uv.x || uv.y != uv.y) 
        return vec3(0.0f);
    //actual uv position
    uv.x = clamp(uv.x, 0.0f, 1.0f);
    uv.y = clamp(uv.y, 0.0f, 1.0f);

    int levelFloor, levelCeiling;
    double dFloor, dCeiling;//distances to actual position
    vec3 cFloor, cCeiling, finalColor; //colors
    double level = calculateLevel(uv);
    
    levelFloor = clamp((int)floor(level), 3, (int)mipmap.size()-1);
    levelCeiling = clamp((int)ceil(level), 4, (int)mipmap.size()-1);
    //cout << "level: " << level << "floor: " << levelFloor << "ceil" << levelCeiling << endl;
    
    cFloor = interpolateLevel(levelFloor, uv);
    if(levelFloor == levelCeiling) return cFloor;

    cCeiling = interpolateLevel(levelCeiling, uv);
    
    dFloor = fabs(levelFloor - level);
    dCeiling = fabs(levelCeiling - level);
    finalColor = linearInterpolate(cFloor, cCeiling, dFloor, dCeiling);
                    
    return finalColor;
}

double TextureSamplerTrilinear::calculateLevel(vec2 uv){
    return log(max(increment.x*width, increment.y*height)) / log(2.0);
}

vec3 TextureSamplerTrilinear::interpolateLevel(int level, vec2 uv) {
    vec2 t1, t2, t3, t4; //texel positions
    double d1, d2, d3, d4, d12, d34;//distances to actual position
    vec3 c1, c2, c3, c4; //colors
    vec3 color12, color34, finalColor;//interpolated colors
                                    
    //actual uv position
    float x = uv.x * (mipmap[level].width-1);
    float y = uv.y * (mipmap[level].height-1);
    
    t1 = vec2(floor(x), ceil(y));
    t2 = vec2(ceil(x),  ceil(y));
    t3 = vec2(floor(x), floor(y));
    t4 = vec2(ceil(x),  floor(y));
    
    
    c1 = mipmap[level].at(t1.x, t1.y);
    c2 = mipmap[level].at(t2.x, t2.y);
    c3 = mipmap[level].at(t3.x, t3.y);
    c4 = mipmap[level].at(t4.x, t4.y);
    

    d1 = fabs(t1.x - x);
    d2 = fabs(t2.x - x);
    color12 = linearInterpolate(c1, c2, d1, d2);

    d3 = fabs(t3.x - x);
    d4 = fabs(t4.x - x);
    color34 = linearInterpolate(c3, c4, d3, d4);
    
    d12 = fabs(t1.y - y);//t1 and t2 have same y
    d34 = fabs(t3.y - y);//t3 and t4 have same y
    finalColor = linearInterpolate(color12, color34, d12, d34);

    return finalColor;
    
}

void TextureSamplerTrilinear::setScanlineIncrement(vec2 increment){
    this->increment = increment;
}