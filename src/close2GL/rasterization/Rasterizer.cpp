#include "Rasterizer.h"

#include <iostream>
using namespace std;

Rasterizer::Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: colorBuffer(colorBuffer), depthBuffer(depthBuffer)
{}

Rasterizer::Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler)
: colorBuffer(colorBuffer), depthBuffer(depthBuffer), sampler(sampler)
{}

vec4 Rasterizer::calculateColorInterpolationStep(vec4 start, vec4 end, float steps) {
    if (steps == 0) return vec4(0.0f);
    vec4 result = (end - start)/steps;
    return result;
}

vec3 Rasterizer::calculateUVInterpolationStep(vec3 start, vec3 end, float steps) {
    if (steps == 0) return vec3(0.0f);
    vec3 difference = end - start;
    return difference / (float) steps;
}
