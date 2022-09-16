#include "RasterizerPoint.h"

RasterizerPoint::RasterizerPoint(ColorBuffer& colorBuffer, DepthBuffer& depthBuffer)
: Rasterizer(colorBuffer, depthBuffer) {}

void RasterizerPoint::rasterize(vector<vec3> vertices, vec3 color){
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
