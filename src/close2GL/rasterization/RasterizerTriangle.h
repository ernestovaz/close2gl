#pragma once
#include "Rasterizer.h"

class RasterizerTriangle : public Rasterizer {
public:
    RasterizerTriangle(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);
    void rasterize(vector<vec3> vertices, vec3 color) override;
    void rasterize(vector<vec4> vertices, vector<vec3> colors) override;

private:
    void initializeArrays(vector<vec3> vertices);
    void initializeArrays(vector<vec4> vertices);

    void updateArrays(int x, int y);
    void updateArrays(int x, int y, vec4 color);

    void fillArrays(vec3 p1, vec3 p2);
    void fillArrays(vec4 p1, vec4 p2, vec3 color1, vec3 color2);

    void calculateDecreasingLine(vec3 start, vec3 end);
    void calculateDecreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor);

    void calculateIncreasingLine(vec3 start, vec3 end);
    void calculateIncreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor);

    void fillTriangle();
    void fillTriangle(vec3 color);
};