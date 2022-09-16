#include "Rasterizer.h"

Rasterizer::Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: colorBuffer(colorBuffer), depthBuffer(depthBuffer)
{}

vec4 Rasterizer::calculateColorInterpolationStep(vec4 start, vec4 end, int steps) {
    vec4 difference = end - start;
    return difference / (float) steps;
}
