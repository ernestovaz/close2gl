#include "RasterizerTriangle.h"


#include <cstdlib>
#include <climits>
#include <algorithm>

#include <glm/glm.hpp>

#include <iostream>
using namespace std;

using std::sort;
using glm::mix;

RasterizerTriangle::RasterizerTriangle(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: Rasterizer(colorBuffer, depthBuffer) {}

RasterizerTriangle::RasterizerTriangle(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler)
: Rasterizer(colorBuffer, depthBuffer, sampler) {}

void RasterizerTriangle::rasterize(vector<vec4> vertices, vec3 color){
    /*
    initializeArrays(vertices);
    fillArrays(vertices[0], vertices[1]);
    fillArrays(vertices[1], vertices[2]);
    fillArrays(vertices[2], vertices[0]);
    fillTriangle(color);
    */
    vector<vec2> uvs = {vec2(0.0f), vec2(0.0f), vec2(0.0f)};
    vector<vec3> colors = {color, color, color};
    newRasterize(vertices, uvs, colors);
}

void RasterizerTriangle::rasterize(vector<vec4> vertices, vector<vec3> colors){
    /*
    initializeArrays(vertices);
    fillArrays(vertices[0], vertices[1], colors[0], colors[1]);
    fillArrays(vertices[1], vertices[2], colors[1], colors[2]);
    fillArrays(vertices[2], vertices[0], colors[2], colors[0]);
    fillTriangle();
    */
    vector<vec2> uvs = {vec2(0.0f), vec2(0.0f), vec2(0.0f)};
    newRasterize(vertices, uvs, colors);
}

void RasterizerTriangle::rasterize(vector<vec4> pos, vector<vec2> uvs, vec3 color) {
    /*
    initializeArrays(pos);
    fillArrays(pos[0], pos[1], uvs[0], uvs[1]);
    fillArrays(pos[1], pos[2], uvs[1], uvs[2]);
    fillArrays(pos[2], pos[0], uvs[2], uvs[0]);
    fillTriangleUsingTexture(color);
    */
    vector<vec3> colors = {color, color, color};
    newRasterize(pos, uvs, colors);
}

void RasterizerTriangle::rasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors) {
    /*
    initializeArrays(pos);
    fillArrays(pos[0], pos[1], uvs[0], uvs[1], colors[0], colors[1]);
    fillArrays(pos[1], pos[2], uvs[1], uvs[2], colors[1], colors[2]);
    fillArrays(pos[2], pos[0], uvs[2], uvs[0], colors[2], colors[0]);
    fillTriangleUsingTexture();
    */
    newRasterize(pos, uvs, colors);
}


Vertex::Vertex(vec4 position, vec2 uv, vec3 color){
    this->position = position;
    this->uv = vec3(uv, 1.0f)/position.w;
    this->color = vec4(color, 1.0f)/position.w;
}


TriangleEdge::TriangleEdge(Vertex top, Vertex bottom)
: top(top), bottom(bottom) {
    float distance = top.position.y - bottom.position.y;

    x = top.position.x;
    step = -(top.position.x - bottom.position.x)/distance;
    
    uv = top.uv;
    uvStep = -(top.uv - bottom.uv)/distance;
    
    color = top.color;
    colorStep = -(top.color - bottom.color)/distance;
}


void RasterizerTriangle::newRasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors) {
    vector<Vertex> vertices;
    for(int i = 0; i < 3; i++)
        vertices.push_back(Vertex(pos[i], uvs[i], colors[i]));

    sort(vertices.begin( ), vertices.end( ), [ ]( const Vertex& v1, const Vertex& v2 ){
        return v1.position.y > v2.position.y;
    });

    TriangleEdge edge1(vertices[0], vertices[1]);
    TriangleEdge edge2(vertices[0], vertices[2]);
    vector<TriangleEdge> remaining = {TriangleEdge(vertices[1], vertices[2])};
    vector<TriangleEdge> active = {edge1};
    if(edge2.x + edge2.step < edge1.x + edge1.step) 
        active.insert(active.begin(), edge2);
    else 
        active.push_back(edge2);

    drawScanlines(active, remaining);   
}


void RasterizerTriangle::drawScanlines(vector<TriangleEdge> active, vector<TriangleEdge> remaining) {
    vec4 color, colorStep;
    vec3 uv, uvStep;
    vec3 finalColor;

    int y = active[0].top.position.y;
    while(true) {

        for(int i=0; i<2; i++){
            if(y < active[i].bottom.position.y){
                if(remaining.empty()) return;
                active[i] = remaining[0];
                remaining.pop_back();
            }
        }
        float distance = active[1].x - active[0].x;

        /*
        color = active[0].color;
        uv = active[0].uv;
        colorStep = calculateColorInterpolationStep(color, active[1].color, distance);
        uvStep = calculateUVInterpolationStep(uv, active[1].uv, distance);
        */

        for(float x = active[0].x; x <= active[1].x; x++) {
            float t = (x-active[0].x)/distance;
            color = mix(active[0].color, active[1].color, t);
            uv = mix(active[0].uv, active[1].uv, t);
            finalColor = vec3(color/color.w);
            if(depthBuffer.update(x, y, -color.w)){
                sampler->setScanlineIncrement(uvStep/uv.z);
                colorBuffer.setColor(x, y, finalColor);
            }
            /*
            color += colorStep;
            if(color.x > active[1].color.x || color.y > active[1].color.y || color.z > active[1].color.z || color.w > active[1].color.w){
                color = active[1].color;
            uv += uvStep;
            */
        }

        for(int i=0; i<2; i++){
            active[i].x += active[i].step;
            active[i].uv += active[i].uvStep;
            active[i].color += active[i].colorStep;
        }

        y--;
    }
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

    minUVs.clear();
    minUVs.reserve(count + 2);
    maxUVs.clear();
    maxUVs.reserve(count + 2);
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

void RasterizerTriangle::updateArrays(int x, int y, vec3 uv) {
    int index = abs(y - minHeight);
    if(x < minValues[index]) {
        minValues[index] = x;
        minUVs[index] = uv;
    }
    if(x > maxValues[index]) {
        maxValues[index] = x;
        maxUVs[index] = uv;
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

void RasterizerTriangle::updateArrays(int x, int y, vec4 color, vec3 uv) {
    int index = abs(y - minHeight);
    if(x < minValues[index]) {
        minValues[index] = x;
        minColors[index] = color;
        minUVs[index] = uv;
    }
    if(x > maxValues[index]) {
        maxValues[index] = x;
        maxColors[index] = color;
        maxUVs[index] = uv;
    }
}

void RasterizerTriangle::fillArrays(vec4 p1, vec4 p2) {
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

void RasterizerTriangle::fillArrays(vec4 p1, vec4 p2, vec2 uv1, vec2 uv2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) calculateDecreasingLine(p2, p1, uv2, uv1);
        else calculateDecreasingLine(p1, p2, uv1, uv2);
    }
    else{
        if(p1.y > p2.y) calculateIncreasingLine(p2, p1, uv2, uv1);
        else calculateIncreasingLine(p1, p2, uv1, uv2);
    }
}

void RasterizerTriangle::fillArrays(vec4 p1, vec4 p2, vec2 uv1, vec2 uv2, vec3 color1, vec3 color2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) calculateDecreasingLine(p2, p1, uv2, uv1, color2, color1);
        else calculateDecreasingLine(p1, p2, uv1, uv2, color1, color2);
    }
    else{
        if(p1.y > p2.y) calculateIncreasingLine(p2, p1, uv2, uv1, color2, color1);
        else calculateIncreasingLine(p1, p2, uv1, uv2, color1, color2);
    }
}

void RasterizerTriangle::calculateDecreasingLine(vec4 start, vec4 end) {
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

void RasterizerTriangle::calculateDecreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    
    float increment = 1;
    if (dY < 0) {
        increment = -1; 
        dY = -dY;
    }
    float difference = (2 * dY) - dX;
    float y = start.y;

    vec3 correctedStartUV = vec3(startUV,1.0)/start.w;
    vec3 correctedEndUV   = vec3(endUV,1.0)/end.w;
    vec3 uvStep = calculateUVInterpolationStep(correctedStartUV, correctedEndUV, dX);
    vec3 uv = correctedStartUV;
    
    for (int x = start.x; x <= end.x; x++) {
        updateArrays(x, y, uv);
        uv += uvStep;
        if (difference > 0) {
            y += increment;
            difference += 2 * (dY - dX);
        } else {
            difference += 2 * dY;
        }
    }
}

void RasterizerTriangle::calculateDecreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 startColor, vec3 endColor) {
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

    vec3 correctedStartUV = vec3(startUV,1.0)/start.w;
    vec3 correctedEndUV   = vec3(endUV,1.0)/end.w;
    vec3 uvStep = calculateUVInterpolationStep(correctedStartUV, correctedEndUV, dX);
    vec3 uv = correctedStartUV;
    
    for (int x = start.x; x <= end.x; x++) {
        updateArrays(x, y, color, uv);
        color += colorStep;
        uv += uvStep;
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

void RasterizerTriangle::calculateIncreasingLine(vec4 start, vec4 end) {
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


void RasterizerTriangle::calculateIncreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV) {
    float dX = end.x - start.x;
    float dY = end.y - start.y;
    
    float increment = 1;
    if (dX < 0) {
        increment = -1; 
        dX = -dX;
    }
    float difference = (2 * dX) - dY;
    float x = start.x;

    vec3 correctedStartUV = vec3(startUV,1.0)/start.w;
    vec3 correctedEndUV   = vec3(endUV,1.0)/end.w;
    vec3 uvStep = calculateUVInterpolationStep(correctedStartUV, correctedEndUV, dY);
    vec3 uv = correctedStartUV;
    
    for (int y = start.y; y <= end.y; y++) {
        updateArrays(x, y, uv);
        uv += uvStep;
        if (difference > 0) {
            x += increment;
            difference += 2 * (dX - dY);
        } else {
            difference += 2 * dX;
        }
    }
}

void RasterizerTriangle::calculateIncreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 startColor, vec3 endColor) {
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

    vec3 correctedStartUV = vec3(startUV,1.0)/start.w;
    vec3 correctedEndUV   = vec3(endUV,1.0)/end.w;
    vec3 uvStep = calculateUVInterpolationStep(correctedStartUV, correctedEndUV, dY);
    vec3 uv = correctedStartUV;
    
    for (int y = start.y; y <= end.y; y++) {
        updateArrays(x, y, color, uv);
        color += colorStep;
        uv += uvStep;
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

void RasterizerTriangle::fillTriangleUsingTexture(vec3 color){
    for (int i = 0; i < minValues.size(); i++) {
        int y = i + minHeight;
        int distance = maxValues[i] - minValues[i];

        vec3 uv = minUVs[i];
        vec3 uvStep = calculateUVInterpolationStep(minUVs[i], maxUVs[i], distance);

        for (int x = minValues[i]; x <= maxValues[i]; x++){
            if(depthBuffer.update(x, y, -uv.z)){
                sampler->setScanlineIncrement(uvStep/uv.z);
                colorBuffer.setColor(x, y, sampler->getColor(uv/uv.z));
            }
            uv += uvStep;
        }
    }
}

void RasterizerTriangle::fillTriangleUsingTexture(){
    for (int i = 0; i < minValues.size(); i++) {
        int y = i + minHeight;
        int distance = maxValues[i] - minValues[i];

        vec4 color = minColors[i];
        vec4 colorStep = calculateColorInterpolationStep(minColors[i], maxColors[i], distance);

        vec3 uv = minUVs[i];
        vec3 uvStep = calculateUVInterpolationStep(minUVs[i], maxUVs[i], distance);
        for (int x = minValues[i]; x <= maxValues[i]; x++) {
            if(depthBuffer.update(x, y, -color.w)){
                sampler->setScanlineIncrement(uvStep/uv.z);
                vec3 shaded = vec3(color/color.w) * sampler->getColor(uv/uv.z) / 255.0f;
                colorBuffer.setColor(x, y, shaded);
            }
            color += colorStep;
            uv += uvStep;
        }
    }
}