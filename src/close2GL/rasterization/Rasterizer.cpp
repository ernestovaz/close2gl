#include "Rasterizer.h"

Rasterizer::Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: colorBuffer(colorBuffer), depthBuffer(depthBuffer)
{}

Rasterizer::Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler)
: colorBuffer(colorBuffer), depthBuffer(depthBuffer), sampler(sampler)
{}

vec4 Rasterizer::calculateColorInterpolationStep(vec4 start, vec4 end, int steps) {
    vec4 difference = end - start;
    return difference / (float) steps;
}

vec3 Rasterizer::calculateUVInterpolationStep(vec3 start, vec3 end, int steps) {
    vec3 difference = end - start;
    return difference / (float) steps;
}
