#pragma once
#include "Rasterizer.h"

class RasterizerLine : public Rasterizer {
public:
    RasterizerLine(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);
    RasterizerLine(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler);

    void rasterize(vector<vec4> pos, vec3 color) override;
    void rasterize(vector<vec4> pos, vector<vec3> colors) override;
    void rasterize(vector<vec4> pos, vector<vec2> uvs, vec3 color) override;
    void rasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors) override;

private:
    void drawLine(vec4 p1, vec4 p2, vec3 color);
    void drawLine(vec4 p1, vec4 p2, vec3 color1, vec3 color2);

    void drawLine(vec4 p1, vec4 p2, vec2 uv1, vec2 uv2, vec3 color);
    void drawLine(vec4 p1, vec4 p2, vec2 uv1, vec2 uv2, vec3 color1, vec3 color2);

    void drawDecreasingLine(vec4 start, vec4 end, vec3 color);
    void drawDecreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor);

    void drawDecreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 color);
    void drawDecreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 startColor, vec3 endColor);

    void drawIncreasingLine(vec4 start, vec4 end, vec3 color);
    void drawIncreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor);

    void drawIncreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 color);
    void drawIncreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 startColor, vec3 endColor);
};