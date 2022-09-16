#pragma once
#include "Rasterizer.h"

class RasterizerLine : public Rasterizer {
public:
    RasterizerLine(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);
    void rasterize(vector<vec3> vertices, vec3 color) override;
    void rasterize(vector<vec4> vertices, vector<vec3> colors) override;

private:
    void drawLine(vec3 p1, vec3 p2, vec3 color);
    void drawLine(vec4 p1, vec4 p2, vec3 color1, vec3 color2);

    void drawDecreasingLine(vec3 start, vec3 end, vec3 color);
    void drawDecreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor);

    void drawIncreasingLine(vec3 start, vec3 end, vec3 color);
    void drawIncreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor);
};