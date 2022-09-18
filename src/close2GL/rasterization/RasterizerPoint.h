#pragma once
#include "Rasterizer.h"

class RasterizerPoint : public Rasterizer {
public:
    RasterizerPoint(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);
    RasterizerPoint(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler);

    void rasterize(vector<vec4> vertices, vec3 color) override;
    void rasterize(vector<vec4> vertices, vector<vec3> colors) override;
    void rasterize(vector<vec4> pos, vector<vec2> uvs, vec3 color) override;
    void rasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors) override;
};