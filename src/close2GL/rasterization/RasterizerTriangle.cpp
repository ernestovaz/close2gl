#include "RasterizerTriangle.h"

#include <cstdlib>
#include <climits>

RasterizerTriangle::RasterizerTriangle(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: Rasterizer(colorBuffer, depthBuffer) {}

void RasterizerTriangle::rasterize(vector<vec3> vertices, vec3 color){
    initializeArrays(vertices);
    fillArrays(vertices[0], vertices[1]);
    fillArrays(vertices[1], vertices[2]);
    fillArrays(vertices[2], vertices[0]);
    fillTriangle(color);
}

void RasterizerTriangle::rasterize(vector<vec4> vertices, vector<vec3> colors){
    initializeArrays(vertices);
    fillArrays(vertices[0], vertices[1], colors[0], colors[1]);
    fillArrays(vertices[1], vertices[2], colors[1], colors[2]);
    fillArrays(vertices[2], vertices[0], colors[2], colors[0]);
    fillTriangle();
}

void RasterizerTriangle::initializeArrays(vector<vec3> vertices) {
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

void RasterizerTriangle::initializeArrays(vector<vec4> vertices) {
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

void RasterizerTriangle::updateArrays(int x, int y) {
    int index = abs(y - minHeight);
    if(x < minValues[index]) {
        minValues[index] = x;
    }
    if(x > maxValues[index]) {
        maxValues[index] = x;
    }
}

void RasterizerTriangle::updateArrays(int x, int y, vec4 color) {
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
void RasterizerTriangle::fillArrays(vec3 p1, vec3 p2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) calculateDecreasingLine(p2, p1);
        else calculateDecreasingLine(p1, p2);
    }
    else{
        if(p1.y > p2.y) calculateIncreasingLine(p2, p1);
        else calculateIncreasingLine(p1, p2);
    }
}

void RasterizerTriangle::fillArrays(vec4 p1, vec4 p2, vec3 color1, vec3 color2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) calculateDecreasingLine(p2, p1, color2, color1);
        else calculateDecreasingLine(p1, p2, color1, color2);
    }
    else{
        if(p1.y > p2.y) calculateIncreasingLine(p2, p1, color2, color1);
        else calculateIncreasingLine(p1, p2, color1, color2);
    }
}

void RasterizerTriangle::calculateDecreasingLine(vec3 start, vec3 end) {
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

void RasterizerTriangle::calculateDecreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor) {
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
    
    vec4 correctedStartColor = vec4(startColor, 1.0)/start.w;
    vec4 correctedEndColor = vec4(endColor, 1.0)/end.w;
    vec4 colorStep = calculateColorInterpolationStep(correctedStartColor, correctedEndColor, dX);
    vec4 color = correctedStartColor;
    
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

void RasterizerTriangle::calculateIncreasingLine(vec3 start, vec3 end) {
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

void RasterizerTriangle::calculateIncreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor) {
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

    
    vec4 correctedStartColor = vec4(startColor, 1.0)/start.w;
    vec4 correctedEndColor = vec4(endColor, 1.0)/end.w;
    vec4 colorStep = calculateColorInterpolationStep(correctedStartColor, correctedEndColor, dY);
    vec4 color = correctedStartColor;
    
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

void RasterizerTriangle::fillTriangle(vec3 color) {
    for (int i = 0; i < minValues.size(); i++) {
        int y = i + minHeight;
        for (int x = minValues[i]; x <= maxValues[i]; x++){
            colorBuffer.setColor(x, y, color);
        }
    }
}

void RasterizerTriangle::fillTriangle() {
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