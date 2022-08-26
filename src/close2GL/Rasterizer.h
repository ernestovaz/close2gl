#pragma once

#include <vector>

#include "ColorBuffer.h"
#include "DepthBuffer.h"
#include <glm/vec4.hpp>

using std::vector;
using glm::vec4;

class Rasterizer {
public:
    Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);

    void rasterizeTriangle(vector<vec3> vertices, vec3 color);
    void rasterizeTriangle(vector<vec3> vertices, vector<vec3> colors, vector<float> wValues);

    void rasterizeLines(vector<vec3> vertices, vec3 color);
    void rasterizeLines(vector<vec3> vertices, vector<vec3> colors, vector<float> wValues);

    void rasterizePoints(vector<vec3> vertices, vec3 color);
    void rasterizePoints(vector<vec3> vertices, vector<vec3> colors);

private:
    ColorBuffer& colorBuffer;
    DepthBuffer& depthBuffer;
    
    int minHeight;
    int maxHeight;
    vector<int> minValues;
    vector<int> maxValues;
    vector<vec4> minColors;
    vector<vec4> maxColors;
    
    void fillArrays(vec3 p1, vec3 p2);
    void fillArrays(vec3 p1, vec3 p2, vec3 color1, vec3 color2, float w1, float w2);
    void calculateDecreasingLine(vec3 start, vec3 end);
    void calculateDecreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor, float startW, float endW);
    void calculateIncreasingLine(vec3 start, vec3 end);
    void calculateIncreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor, float startW, float endW);
    void fillTriangle(vec3 color);
    void fillTriangle();
    void fillTriangle(vector<vec3> color);

    void drawLine(vec3 p1, vec3 p2, vec3 color);
    void drawLine(vec3 p1, vec3 p2, vec3 color1, vec3 color2, float w1, float w2);
    void drawDecreasingLine(vec3 start, vec3 end, vec3 color);
    void drawDecreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor, float startW, float endW);
    void drawIncreasingLine(vec3 start, vec3 end, vec3 color);
    void drawIncreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor, float startW, float endW);
    
    void initializeArrays(vector<vec3> vertices);
    void updateArrays(int x, int y);
    void updateArrays(int x, int y, vec4 color);
    
    vec4 calculateColorInterpolationStep(vec4 start, vec4 end, int steps);
};