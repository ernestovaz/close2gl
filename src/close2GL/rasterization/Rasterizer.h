#pragma once

#include <vector>

#include "../buffer/ColorBuffer.h"
#include "../buffer/DepthBuffer.h"
#include "../texture/TextureSampler.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using std::vector;
using glm::vec2;
using glm::vec3;
using glm::vec4;

class Rasterizer {
public:
    Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);
    Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler);

    virtual void rasterize(vector<vec4> pos, vec3 color){}
    virtual void rasterize(vector<vec4> pos, vector<vec3> colors){}
    virtual void rasterize(vector<vec4> pos, vector<vec2> uvs, vec3 color){}
    virtual void rasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors){}

protected:
    ColorBuffer& colorBuffer;
    DepthBuffer& depthBuffer;
    TextureSampler* sampler;

    vector<int> minValues;
    vector<int> maxValues;
    vector<vec4> minColors;
    vector<vec4> maxColors;
    vector<vec3> minUVs;
    vector<vec3> maxUVs;
    int minHeight;
    int maxHeight;
    
    vec4 calculateColorInterpolationStep(vec4 start, vec4 end, int steps);
    vec3 calculateUVInterpolationStep(vec3 start, vec3 end, int steps);
};