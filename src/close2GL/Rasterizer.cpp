#include "Rasterizer.h"

#include <cstdlib>
#include <ctime>
#include <climits>
#include <iostream>

Rasterizer::Rasterizer(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: colorBuffer(colorBuffer), depthBuffer(depthBuffer)
{}

void Rasterizer::rasterizeTriangle(vector<vec3> vertices, vec3 color) {
    initializeArrays(vertices);
    fillArrays(vertices[0], vertices[1]);
    fillArrays(vertices[1], vertices[2]);
    fillArrays(vertices[2], vertices[0]);
    fillTriangle(color);
}

void Rasterizer::rasterizeTriangle(vector<vec3> vertices, vector<vec3> colors, vector<float> wValues) {
    initializeArrays(vertices);
    fillArrays(vertices[0], vertices[1], colors[0], colors[1], wValues[0], wValues[1]);
    fillArrays(vertices[1], vertices[2], colors[1], colors[2], wValues[1], wValues[2]);
    fillArrays(vertices[2], vertices[0], colors[2], colors[0], wValues[2], wValues[0]);
    fillTriangle();
}

void Rasterizer::rasterizeLines(vector<vec3> vertices, vec3 color) {
    drawLine(vertices[0], vertices[1], color);
    drawLine(vertices[1], vertices[2], color);
    drawLine(vertices[2], vertices[0], color);
}

void Rasterizer::rasterizeLines(vector<vec3> vertices, vector<vec3> colors, vector<float> wValues) {
    drawLine(vertices[0], vertices[1], colors[0], colors[1], wValues[0], wValues[1]);
    drawLine(vertices[1], vertices[2], colors[1], colors[2], wValues[1], wValues[2]);
    drawLine(vertices[2], vertices[0], colors[2], colors[0], wValues[2], wValues[0]);
}

void Rasterizer::rasterizePoints(vector<vec3> vertices, vec3 color) {
    for(vec3 vertex : vertices){
        colorBuffer.setColor(vertex.x, vertex.y, color);
    }
}

void Rasterizer::rasterizePoints(vector<vec3> vertices, vector<vec3> colors) {
    for(int i = 0; i <= 2; i++){
        if(depthBuffer.update(vertices[i].x, vertices[i].y, vertices[i].z))
            colorBuffer.setColor(vertices[i].x, vertices[i].y, colors[i]);
    }
}

void Rasterizer::fillArrays(vec3 p1, vec3 p2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) calculateDecreasingLine(p2, p1);
        else calculateDecreasingLine(p1, p2);
    }
    else{
        if(p1.y > p2.y) calculateIncreasingLine(p2, p1);
        else calculateIncreasingLine(p1, p2);
    }
}

void Rasterizer::fillArrays(vec3 p1, vec3 p2, vec3 color1, vec3 color2, float w1, float w2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) calculateDecreasingLine(p2, p1, color2, color1, w2, w1);
        else calculateDecreasingLine(p1, p2, color1, color2, w1, w2);
    }
    else{
        if(p1.y > p2.y) calculateIncreasingLine(p2, p1, color2, color1, w2, w1);
        else calculateIncreasingLine(p1, p2, color1, color2, w1, w2);
    }
}

void Rasterizer::calculateDecreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor, float startW, float endW) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    float dZ = end.z - start.z;
    
    float yIncrement = 1;
    if (dY < 0) {
        yIncrement = -1; 
        dY = -dY;
    }
    float yDifference = (2 * dY) - dX;
    float y = start.y;
    
    float zIncrement = 1;
    if (dZ < 0) {
        zIncrement = -1; 
        dZ = -dZ;
    }
    float zDifference = (2 * dZ) - dX;
    float z = start.z;
    
    vec4 h_startColor   = vec4(startColor, 1.0)/startW;
    vec4 h_endColor     = vec4(endColor, 1.0)/endW;
    vec4 colorStep = calculateColorInterpolationStep(h_startColor, h_endColor, dX);
    vec4 color = h_startColor;
    
    for (int x = start.x; x <= end.x; x++) {
        updateArrays(x, y, color);
        color += colorStep;
        if (yDifference > 0) {
            y += yIncrement;
            yDifference += 2 * (dY - dX);
        } else {
            yDifference += 2 * dY;
        }
        if (zDifference > 0) {
            z += zIncrement;
            zDifference += 2 * (dZ - dX);
        } else {
            zDifference += 2 * dZ;
        }
    }
}

void Rasterizer::calculateIncreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor, float startW, float endW) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    float dZ = end.z - start.z;
    
    float increment = 1;
    if (dX < 0) {
        increment = -1; 
        dX = -dX;
    }
    float difference = (2 * dX) - dY;
    float x = start.x;

    float zIncrement = 1;
    if (dZ < 0) {
        zIncrement = -1; 
        dZ = -dZ;
    }
    float zDifference = (2 * dZ) - dX;
    float z = start.z;

    
    vec4 h_startColor   = vec4(startColor, 1.0)/startW;
    vec4 h_endColor     = vec4(endColor, 1.0)/endW;
    vec4 colorStep = calculateColorInterpolationStep(h_startColor, h_endColor, dY);
    vec4 color = h_startColor;
    
    for (int y = start.y; y <= end.y; y++) {
        updateArrays(x, y, color);
        color += colorStep;
        if (difference > 0) {
            x += increment;
            difference += 2 * (dX - dY);
        } else {
            difference += 2 * dX;
        }
        if (zDifference > 0) {
            z += zIncrement;
            zDifference += 2 * (dZ - dX);
        } else {
            zDifference += 2 * dZ;
        }
    }
}

void Rasterizer::calculateDecreasingLine(vec3 start, vec3 end) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    
    float increment = 1;
    if (dY < 0) {
        increment = -1; 
        dY = -dY;
    }
    float difference = (2 * dY) - dX;
    float y = start.y;
    
    for (int x = start.x; x <= end.x; x++) {
        updateArrays(x, y);
        if (difference > 0) {
            y += increment;
            difference += 2 * (dY - dX);
        } else {
            difference += 2 * dY;
        }
    }
}

void Rasterizer::calculateIncreasingLine(vec3 start, vec3 end) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    
    float increment = 1;
    if (dX < 0) {
        increment = -1; 
        dX = -dX;
    }
    float difference = (2 * dX) - dY;
    float x = start.x;
    
    for (int y = start.y; y <= end.y; y++) {
        updateArrays(x, y);
        if (difference > 0) {
            x += increment;
            difference += 2 * (dX - dY);
        } else {
            difference += 2 * dX;
        }
    }
}

void Rasterizer::fillTriangle(vec3 color) {
    for (int i = 0; i < minValues.size(); i++) {
        int y = i + minHeight;
        for (int x = minValues[i]; x <= maxValues[i]; x++)
            colorBuffer.setColor(x, y, color);
    }
}

void Rasterizer::fillTriangle() {
    for (int i = 0; i < minValues.size(); i++) {
        int y = i + minHeight;
        int distance = maxValues[i] - minValues[i];
        vec4 color = minColors[i];
        vec4 step = calculateColorInterpolationStep(minColors[i], maxColors[i], distance);
        for (int x = minValues[i]; x <= maxValues[i]; x++) {
            if(depthBuffer.update(x, y, -color.w))
                colorBuffer.setColor(x, y, color/color.w);
            color += step;
        }
    }
}

void Rasterizer::initializeArrays(vector<vec3> vertices) {
    minHeight = vertices[0].y;
    maxHeight = vertices[0].y;
    for(int i = 0; i <= 2; i++) {
        if(vertices[i].y < minHeight) minHeight = vertices[i].y;
        if(vertices[i].y > maxHeight) maxHeight = vertices[i].y;
    }
    int count = abs(maxHeight - minHeight);
    minValues.assign(count + 2, INT_MAX);
    maxValues.assign(count + 2, INT_MIN);
    minColors.clear();
    minColors.reserve(count + 2);
    maxColors.clear();
    maxColors.reserve(count + 2);
}

void Rasterizer::updateArrays(int x, int y) {
    int index = abs(y - minHeight);
    if(x < minValues[index]) {
        minValues[index] = x;
    }
    if(x > maxValues[index]) {
        maxValues[index] = x;
    }
}

void Rasterizer::updateArrays(int x, int y, vec4 color) {
    int index = abs(y - minHeight);
    if(x < minValues[index]) {
        minValues[index] = x;
        minColors[index] = color;
    }
    if(x > maxValues[index]) {
        maxValues[index] = x;
        maxColors[index] = color;
    }
}

void Rasterizer::drawLine(vec3 p1, vec3 p2, vec3 color) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) drawDecreasingLine(p2, p1, color);
        else drawDecreasingLine(p1, p2, color);
    }
    else{
        if(p1.y > p2.y) drawIncreasingLine(p2, p1, color);
        else drawIncreasingLine(p1, p2, color);
    }
}

void Rasterizer::drawDecreasingLine(vec3 start, vec3 end, vec3 color) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    
    float increment = 1;
    if (dY < 0) {
        increment = -1; 
        dY = -dY;
    }
    float difference = (2 * dY) - dX;
    float y = start.y;
    
    for (int x = start.x; x <= end.x; x++) {
        colorBuffer.setColor(x, y, color);
        if (difference > 0) {
            y += increment;
            difference += 2 * (dY - dX);
        } else {
            difference += 2 * dY;
        }
    }
}

void Rasterizer::drawIncreasingLine(vec3 start, vec3 end, vec3 color) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    
    float increment = 1;
    if (dX < 0) {
        increment = -1; 
        dX = -dX;
    }
    float difference = (2 * dX) - dY;
    float x = start.x;
    
    for (int y = start.y; y <= end.y; y++) {
        colorBuffer.setColor(x, y, color);
        if (difference > 0) {
            x += increment;
            difference += 2 * (dX - dY);
        } else {
            difference += 2 * dX;
        }
    }
}

void Rasterizer::drawLine(vec3 p1, vec3 p2, vec3 color1, vec3 color2, float w1, float w2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) drawDecreasingLine(p2, p1, color2, color1, w2, w1);
        else drawDecreasingLine(p1, p2, color1, color2, w1, w2);
    }
    else{
        if(p1.y > p2.y) drawIncreasingLine(p2, p1, color2, color1, w2, w1);
        else drawIncreasingLine(p1, p2, color1, color2, w1, w2);
    }
}

void Rasterizer::drawDecreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor, float startW, float endW) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    float dZ = end.z - start.z;
    
    float yIncrement = 1;
    if (dY < 0) {
        yIncrement = -1; 
        dY = -dY;
    }
    float yDifference = (2 * dY) - dX;
    float y = start.y;
    
    float zIncrement = 1;
    if (dZ < 0) {
        zIncrement = -1; 
        dZ = -dZ;
    }
    float zDifference = (2 * dZ) - dX;
    float z = start.z;
    
    vec4 h_startColor   = vec4(startColor, 1.0)/startW;
    vec4 h_endColor     = vec4(endColor, 1.0)/endW;
    vec4 colorStep = calculateColorInterpolationStep(h_startColor, h_endColor, dX);
    vec4 color = h_startColor;
    
    for (int x = start.x; x <= end.x; x++) {
        if(depthBuffer.update(x, y, z))
            colorBuffer.setColor(x, y, color/color.w);
        color += colorStep;
        if (yDifference > 0) {
            y += yIncrement;
            yDifference += 2 * (dY - dX);
        } else {
            yDifference += 2 * dY;
        }
        if (zDifference > 0) {
            z += zIncrement;
            zDifference += 2 * (dZ - dX);
        } else {
            zDifference += 2 * dZ;
        }
    }
}

void Rasterizer::drawIncreasingLine(vec3 start, vec3 end, vec3 startColor, vec3 endColor, float startW, float endW) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    float dZ = end.z - start.z;
    
    float increment = 1;
    if (dX < 0) {
        increment = -1; 
        dX = -dX;
    }
    float difference = (2 * dX) - dY;
    float x = start.x;

    float zIncrement = 1;
    if (dZ < 0) {
        zIncrement = -1; 
        dZ = -dZ;
    }
    float zDifference = (2 * dZ) - dX;
    float z = start.z;

    
    vec4 h_startColor   = vec4(startColor, 1.0)/startW;
    vec4 h_endColor     = vec4(endColor, 1.0)/endW;
    vec4 colorStep = calculateColorInterpolationStep(h_startColor, h_endColor, dY);
    vec4 color = h_startColor;
    
    for (int y = start.y; y <= end.y; y++) {
        if(depthBuffer.update(x, y, z))
            colorBuffer.setColor(x, y, color/color.w);
        color += colorStep;
        if (difference > 0) {
            x += increment;
            difference += 2 * (dX - dY);
        } else {
            difference += 2 * dX;
        }
        if (zDifference > 0) {
            z += zIncrement;
            zDifference += 2 * (dZ - dX);
        } else {
            zDifference += 2 * dZ;
        }
    }
}

vec4 Rasterizer::calculateColorInterpolationStep(vec4 start, vec4 end, int steps) {
    vec4 difference = end - start;
    return difference / (float) steps;
}