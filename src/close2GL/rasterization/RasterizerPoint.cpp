#include "RasterizerPoint.h"


RasterizerPoint::RasterizerPoint(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: Rasterizer(colorBuffer, depthBuffer) {}

RasterizerPoint::RasterizerPoint(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer, TextureSampler* sampler)
: Rasterizer(colorBuffer, depthBuffer, sampler) {}

void RasterizerPoint::rasterize(vector<vec4> vertices, vec3 color){
    for(vec3 vertex : vertices){
        colorBuffer.setColor(vertex.x, vertex.y, color);
    }
}

void RasterizerPoint::rasterize(vector<vec4> vertices, vector<vec3> colors){
    for(int i = 0; i <= 2; i++){
        if(depthBuffer.update(vertices[i].x, vertices[i].y, vertices[i].z))
            colorBuffer.setColor(vertices[i].x, vertices[i].y, colors[i]);
    }
}

void RasterizerPoint::rasterize(vector<vec4> pos, vector<vec2> uvs, vec3 color) {
    for(int i = 0; i <= 2; i++){
        if(depthBuffer.update(pos[i].x, pos[i].y, pos[i].z)){
            colorBuffer.setColor(pos[i].x, pos[i].y, (color/255.0f) * sampler->getColor(uvs[i]));
        }
    }
}

void RasterizerPoint::rasterize(vector<vec4> pos, vector<vec2> uvs, vector<vec3> colors) {
    for(int i = 0; i <= 2; i++){
        if(depthBuffer.update(pos[i].x, pos[i].y, pos[i].z)){
            vec3 shaded = colors[i];
            shaded *= sampler->getColor(uvs[i]);
            colorBuffer.setColor(pos[i].x, pos[i].y, shaded);
        }
    }
}