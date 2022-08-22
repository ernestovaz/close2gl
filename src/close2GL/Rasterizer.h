#pragma once

#include <vector>

#include "ColorBuffer.h"
#include "DepthBuffer.h"

using std::vector;

class Rasterizer {
public:
    Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer);

    void rasterizeTriangle(vector<vec3> vertices, vec3 color);
    void rasterizeTriangle(vector<vec3> vertices, vector<vec3> colors);

    void rasterizeLines(vector<vec3> vertices, vec3 color);
    void rasterizeLines(vector<vec3> vertices, vector<vec3> colors);

    void rasterizePoints(vector<vec3> vertices, vec3 color);
    void rasterizePoints(vector<vec3> vertices, vector<vec3> colors);

private:
    ColorBuffer& colorBuffer;
    DepthBuffer& depthBuffer;
    
    int minHeight;
    int maxHeight;
    vector<int> minValues;
    vector<int> maxValues;
    
    void fillArrays(vec3 p1, vec3 p2);
    void calculateDecreasingLine(vec3 start, vec3 end);
    void calculateIncreasingLine(vec3 start, vec3 end);
    void fillTriangle(vec3 color);
    void fillTriangle(vector<vec3> color);

    void drawLine(vec3 p1, vec3 p2, vec3 color);
    void drawLine(vec3 p1, vec3 p2, vec3 color1, vec3 color2);
    void drawDecreasingLine(vec3 start, vec3 end, vec3 color);
    void drawDecreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor);
    void drawIncreasingLine(vec3 start, vec3 end, vec3 color);
    void drawIncreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor);
    
    void initializeArrays(vector<vec3> vertices);
    void updateArrays(int x, int y);
    
    vec3 calculateColorInterpolationStep(vec3 start, vec3 end, int steps);
};