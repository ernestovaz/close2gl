#pragma once

#include <vector>

#include "../buffer/ColorBuffer.h"
#include "../buffer/DepthBuffer.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using std::vector;
using glm::vec3;
using glm::vec4;

class Rasterizer {
public:
    Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);
    virtual void rasterize(vector<vec3> vertices, vec3 color){}
    virtual void rasterize(vector<vec4> vertices, vector<vec3> colors){}
protected:
    ColorBuffer& colorBuffer;
    DepthBuffer& depthBuffer;

    vector<int> minValues;
    vector<int> maxValues;
    vector<vec4> minColors;
    vector<vec4> maxColors;
    int minHeight;
    int maxHeight;
    
    vec4 calculateColorInterpolationStep(vec4 start, vec4 end, int steps);
};