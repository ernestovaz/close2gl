#include "Rasterizer.h"

#include <cstdlib>
#include <climits>

Rasterizer::Rasterizer(ColorBuffer& buffer, vec3 color)
: buffer(buffer), color(color)
{}

void Rasterizer::rasterizeTriangle(vector<vec3> vertices) {
    initializeArrays(vertices);
    fillArrays(vertices[0], vertices[1]);
    fillArrays(vertices[1], vertices[2]);
    fillArrays(vertices[2], vertices[0]);
    fillTriangle();
}

void Rasterizer::rasterizeLines(vector<vec3> vertices) {
    drawLine(vertices[0], vertices[1]);
    drawLine(vertices[1], vertices[2]);
    drawLine(vertices[2], vertices[0]);
}

void Rasterizer::rasterizePoints(vector<vec3> vertices) {
    for(vec3 vertex : vertices){
        buffer.setColor(vertex.x, vertex.y, color);
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

void Rasterizer::fillTriangle() {
    for (int i = 0; i < minValues.size(); i++) {
        int y = i + minHeight;
        for (int x = minValues[i]; x <= maxValues[i]; x++)
            buffer.setColor(x, y, color);
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

void Rasterizer::drawLine(vec3 p1, vec3 p2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) drawDecreasingLine(p2, p1);
        else drawDecreasingLine(p1, p2);
    }
    else{
        if(p1.y > p2.y) drawIncreasingLine(p2, p1);
        else drawIncreasingLine(p1, p2);
    }
}

void Rasterizer::drawDecreasingLine(vec3 start, vec3 end) {
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
        buffer.setColor(x, y, color);
        if (difference > 0) {
            y += increment;
            difference += 2 * (dY - dX);
        } else {
            difference += 2 * dY;
        }
    }
}

void Rasterizer::drawIncreasingLine(vec3 start, vec3 end) {
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
        buffer.setColor(x, y, color);
        if (difference > 0) {
            x += increment;
            difference += 2 * (dX - dY);
        } else {
            difference += 2 * dX;
        }
    }
}