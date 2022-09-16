#pragma once
#include "Rasterizer.h"

class RasterizerPoint : public Rasterizer {
public:
    RasterizerPoint(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);
    void rasterize(vector<vec3> vertices, vec3 color) override;
    void rasterize(vector<vec4> vertices, vector<vec3> colors) override;
};