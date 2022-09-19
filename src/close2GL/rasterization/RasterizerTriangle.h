#pragma once
#include "Rasterizer.h"

class Vertex {
public:
    vec4 position;
    vec3 uv;
    vec4 color;

    Vertex(vec4 position, vec2 uv, vec3 color);
};

class TriangleEdge {
public:
    Vertex top;
    Vertex bottom;

    float x;
    float step;
    
    vec3 uv;
    vec3 uvStep;
    
    vec4 color;
    vec4 colorStep;

    TriangleEdge(Vertex top, Vertex bottom);
};

class RasterizerTriangle : public Rasterizer {
public:
    RasterizerTriangle(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);
    RasterizerTriangle(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler);
    void rasterize(vector<vec4> vertices, vec3 color) override;
    void rasterize(vector<vec4> vertices, vector<vec3> colors) override;
    void rasterize(vector<vec4> pos, vector<vec2> uvs, vec3 color) override;
    void rasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors) override;
    void newRasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors);

private:
    void drawScanlines(vector<TriangleEdge> active, vector<TriangleEdge> remaining);
    void initializeArrays(vector<vec4> vertices);

    void updateArrays(int x, int y);
    void updateArrays(int x, int y, vec3 uv);
    void updateArrays(int x, int y, vec4 color);
    void updateArrays(int x, int y, vec4 color, vec3 uv);

    void fillArrays(vec4 p1, vec4 p2);
    void fillArrays(vec4 p1, vec4 p2, vec2 uv1, vec2 uv2);
    void fillArrays(vec4 p1, vec4 p2, vec3 color1, vec3 color2);
    void fillArrays(vec4 p1, vec4 p2, vec2 uv1, vec2 uv2, vec3 color1, vec3 color2);

    void calculateDecreasingLine(vec4 start, vec4 end);
    void calculateDecreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV);
    void calculateDecreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 startColor, vec3 endColor);
    void calculateDecreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor);

    void calculateIncreasingLine(vec4 start, vec4 end);
    void calculateIncreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor);
    void calculateIncreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV);
    void calculateIncreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 startColor, vec3 endColor);

    void fillTriangle();
    void fillTriangle(vec3 color);
    void fillTriangleUsingTexture();
    void fillTriangleUsingTexture(vec3 color);
};
