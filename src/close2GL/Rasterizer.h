#pragma once

#include <vector>

#include "ColorBuffer.h"

using std::vector;

class Rasterizer {
public:
    Rasterizer(ColorBuffer& buffer, vec3 color);
    void rasterizeTriangle(vector<vec3> vertices);
    void rasterizeLines(vector<vec3> vertices);
    void rasterizePoints(vector<vec3> vertices);

private:
    ColorBuffer& buffer;
    vec3 color;
    
    int minHeight;
    int maxHeight;
    vector<int> minValues;
    vector<int> maxValues;
    
    void fillArrays(vec3 p1, vec3 p2);
    void calculateDecreasingLine(vec3 start, vec3 end);
    void calculateIncreasingLine(vec3 start, vec3 end);
    void fillTriangle();

    void drawLine(vec3 p1, vec3 p2);
    void drawDecreasingLine(vec3 start, vec3 end);
    void drawIncreasingLine(vec3 start, vec3 end);
    
    void initializeArrays(vector<vec3> vertices);
    void updateArrays(int x, int y);
};