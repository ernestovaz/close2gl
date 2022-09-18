#include "TextureSamplerBilinear.h"

#include <cmath>

#include <iostream>
using namespace std;

TextureSamplerBilinear::TextureSamplerBilinear(unsigned char* data, int width, int height)
: TextureSampler(data, width, height) {}

vec3 TextureSamplerBilinear::getColor(vec2 uv){
    if(uv.x != uv.x || uv.y != uv.y) 
        return vec3(0.0f);

    vec2 t1, t2, t3, t4; //texel positions
    double d1, d2, d3, d4, d12, d34;//distances to actual position
    vec3 c1, c2, c3, c4; //colors
    vec3 color12, color34, finalColor;//interpolated colors
                                      
    //actual uv position
    double x = uv.x * width;
    double y = uv.y * height;

    t1 = vec2(floor(x), ceil(y));
    t2 = vec2(ceil(x),  ceil(y));
    t3 = vec2(floor(x), floor(y));
    t4 = vec2(ceil(x),  floor(y));
    
    c1 = getTexelColor(t1);
    c2 = getTexelColor(t2);
    c3 = getTexelColor(t3);
    c4 = getTexelColor(t4);

    d1 = fabs(t1.x - x);
    d2 = fabs(t2.x - x);
    color12 = linearInterpolate(c1, c2, d1, d2);

    d3 = fabs(t3.x - x);
    d4 = fabs(t4.x - x);
    color34 = linearInterpolate(c3, c4, d3, d4);
    
    d12 = fabs(t1.y - y);//t1 and t2 have same y
    d34 = fabs(t3.y - y);//t3 and t4 have same y
    finalColor= linearInterpolate(color12, color34, d12, d34);

    return finalColor;
}