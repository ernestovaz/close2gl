#include "RasterizerLine.h"

#include <cstdlib>

RasterizerLine::RasterizerLine(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: Rasterizer(colorBuffer, depthBuffer) {}

RasterizerLine::RasterizerLine(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler)
: Rasterizer(colorBuffer, depthBuffer, sampler) {}

void RasterizerLine::rasterize(vector<vec4> pos, vec3 color){
    drawLine(pos[0], pos[1], color);
    drawLine(pos[1], pos[2], color);
    drawLine(pos[2], pos[0], color);
}

void RasterizerLine::rasterize(vector<vec4> pos, vector<vec3> colors){
    drawLine(pos[0], pos[1], colors[0], colors[1]);
    drawLine(pos[1], pos[2], colors[1], colors[2]);
    drawLine(pos[2], pos[0], colors[2], colors[0]);
}

void RasterizerLine::rasterize(vector<vec4> pos, vector<vec2> uvs, vec3 color) {
    drawLine(pos[0], pos[1], uvs[0], uvs[1], color);
    drawLine(pos[1], pos[2], uvs[1], uvs[2], color);
    drawLine(pos[2], pos[0], uvs[2], uvs[0], color);
}

void RasterizerLine::rasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors) {
    drawLine(pos[0], pos[1], uvs[0], uvs[1], colors[0], colors[1]);
    drawLine(pos[1], pos[2], uvs[1], uvs[2], colors[1], colors[2]);
    drawLine(pos[2], pos[0], uvs[2], uvs[0], colors[2], colors[0]);
}

void RasterizerLine::drawLine(vec4 p1, vec4 p2, vec3 color) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) drawDecreasingLine(p2, p1, color);
        else drawDecreasingLine(p1, p2, color);
    }
    else{
        if(p1.y > p2.y) drawIncreasingLine(p2, p1, color);
        else drawIncreasingLine(p1, p2, color);
    }
}

void RasterizerLine::drawLine(vec4 p1, vec4 p2, vec3 color1, vec3 color2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) drawDecreasingLine(p2, p1, color2, color1);
        else drawDecreasingLine(p1, p2, color1, color2);
    }
    else{
        if(p1.y > p2.y) drawIncreasingLine(p2, p1, color2, color1);
        else drawIncreasingLine(p1, p2, color1, color2);
    }
}

void RasterizerLine::drawLine(vec4 p1, vec4 p2, vec2 uv1, vec2 uv2, vec3 color) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) drawDecreasingLine(p2, p1, uv2, uv1, color);
        else drawDecreasingLine(p1, p2, uv1, uv2, color);
    }
    else{
        if(p1.y > p2.y) drawIncreasingLine(p2, p1, uv1, uv2, color);
        else drawIncreasingLine(p1, p2, uv1, uv2, color);
    }
}

void RasterizerLine::drawLine(vec4 p1, vec4 p2, vec2 uv1, vec2 uv2, vec3 color1, vec3 color2) {
    if(abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        if(p1.x > p2.x) drawDecreasingLine(p2, p1, uv2, uv1, color2, color1);
        else drawDecreasingLine(p1, p2, uv1, uv2, color1, color2);
    }
    else{
        if(p1.y > p2.y) drawIncreasingLine(p2, p1, uv2, uv1, color2, color1);
        else drawIncreasingLine(p1, p2, uv1, uv2, color1, color2);
    }
}

void RasterizerLine::drawDecreasingLine(vec4 start, vec4 end, vec3 color) {
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

void RasterizerLine::drawDecreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor) {
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
    
    vec4 correctedStartColor = vec4(startColor,1.0)/start.w;
    vec4 correctedEndColor   = vec4(endColor,1.0)/end.w;
    vec4 colorStep = calculateColorInterpolationStep(correctedStartColor, correctedEndColor, dX);
    vec4 color = correctedStartColor;
    
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

void RasterizerLine::drawDecreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 color) {
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
        vec3 shaded = color * sampler->getColor(uv/uv.z);
        colorBuffer.setColor(x, y, shaded);
        uv += uvStep;
        if (difference > 0) {
            y += increment;
            difference += 2 * (dY - dX);
        } else {
            difference += 2 * dY;
        }
    }
}

void RasterizerLine::drawDecreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 startColor, vec3 endColor) {
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
    
    vec4 correctedStartColor = vec4(startColor,1.0)/start.w;
    vec4 correctedEndColor   = vec4(endColor,1.0)/end.w;
    vec4 colorStep = calculateColorInterpolationStep(correctedStartColor, correctedEndColor, dX);
    vec4 color = correctedStartColor;

    vec3 correctedStartUV = vec3(startUV,1.0)/start.w;
    vec3 correctedEndUV   = vec3(endUV,1.0)/end.w;
    vec3 uvStep = calculateUVInterpolationStep(correctedStartUV, correctedEndUV, dX);
    vec3 uv = correctedStartUV;
    
    
    for (int x = start.x; x <= end.x; x++) {
        if(depthBuffer.update(x, y, z)){
            vec3 shaded = vec3(color/color.w) * sampler->getColor(uv/uv.z);
            colorBuffer.setColor(x, y, shaded);
        }
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

void RasterizerLine::drawIncreasingLine(vec4 start, vec4 end, vec3 startColor, vec3 endColor) {
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

void RasterizerLine::drawIncreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 color) {
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
        vec3 shaded = color * sampler->getColor(uv/uv.z);
        colorBuffer.setColor(x, y, shaded);
        uv += uvStep;
        if (difference > 0) {
            x += increment;
            difference += 2 * (dX - dY);
        } else {
            difference += 2 * dX;
        }
    }
}

void RasterizerLine::drawIncreasingLine(vec4 start, vec4 end, vec2 startUV, vec2 endUV, vec3 startColor, vec3 endColor) {
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
        if(depthBuffer.update(x, y, z)){
            vec3 shaded = vec3(color/color.w) * sampler->getColor(uv/uv.z);
            colorBuffer.setColor(x, y, shaded);
        }
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

void RasterizerLine::drawIncreasingLine(vec4 start, vec4 end, vec3 color) {
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
