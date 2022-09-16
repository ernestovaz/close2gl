#include "Close2GL.h"

#include "rasterization/RasterizerPoint.h"
#include "rasterization/RasterizerLine.h"
#include "rasterization/RasterizerTriangle.h"

#include "Shader.h"

#include <iostream>
#include <glm/vec4.hpp>

using std::cout;
using std::endl;
using glm::vec4;

Close2GL::Primitive Close2GL::primitive = TRIANGLES;
Close2GL::Shading Close2GL::shading = NO_SHADING;
Close2GL::Orientation Close2GL::frontFace = CCW;

vec3 Close2GL::color = vec3(0.0f, 0.0f, 0.0f);
bool Close2GL::useTexture = false;
bool Close2GL::useHorizontalFOV = false;
bool Close2GL::performBackfaceCulling = false;

vec3 Close2GL::cameraPosition = vec3(0.0f, 0.0f, 0.0f);
vec3 Close2GL::cameraDirection = vec3(0.0f, 0.0f,-1.0f);
vec3 Close2GL::cameraUp = vec3(0.0f, 1.0f, 0.0f);

float Close2GL::verticalFOV = 60;
float Close2GL::horizontalFOV = 60;
float Close2GL::near = 0.01;
float Close2GL::far = 10;

ColorBuffer Close2GL::colorBuffer; 
DepthBuffer Close2GL::depthBuffer; 

mat4 Close2GL::model; 
mat4 Close2GL::view; 
mat4 Close2GL::projection; 
mat4 Close2GL::viewport = mat4(0.0f); 

float Close2GL::width;
float Close2GL::height;

vector<float> Close2GL::WValues;


void Close2GL::clear(vec3 color){
    colorBuffer.clear(color);
    depthBuffer.clear();
}

void Close2GL::updateViewport(int left, int top, int right, int bottom){
    viewport = mat4(0.0f); 

    //scaling
    viewport[0][0] = (right - left)/2.0f;
    viewport[1][1] = (bottom - top)/2.0f;
    viewport[2][2] = 1;
    viewport[3][3] = 1;

    //translation
    viewport[3][0] = (right + left)/2.0f;
    viewport[3][1] = (bottom + top)/2.0f;
    
    width = right-left;
    height = bottom-top;
    colorBuffer.resize(width, height);
    depthBuffer.resize(width, height);
}

void Close2GL::draw(vector<unsigned int> ids, vector<vec3> pos, vector<vec3> norms, vector<vec2> uvs) {
    updateView();
    updateProjection();

    vector<vec3> cameraPositions = transform(pos, view, false);
    vector<vec3> projectedPositions = transform(cameraPositions, projection, true);
    vector<unsigned int> culledIndices = viewFrustumCull(ids, projectedPositions);

    vector<vec3> screenPositions = transform(projectedPositions, viewport, false);
    if(performBackfaceCulling){
        culledIndices = backfaceCull(culledIndices, screenPositions);
    }
    
    int count = culledIndices.size();
    Rasterizer* rasterizer = NULL;
    switch(primitive){
        case POINTS:
            rasterizer = new RasterizerPoint(colorBuffer, depthBuffer);
            break;
        case LINES:
            rasterizer = new RasterizerLine(colorBuffer, depthBuffer);
            break;
        case TRIANGLES:
            rasterizer = new RasterizerTriangle(colorBuffer, depthBuffer);
            break;
    }
    switch (shading){
        case NO_SHADING:
            drawNoShading(culledIndices, screenPositions, uvs, rasterizer);
            break;
        case GOURAUD:
            drawGouraud(culledIndices, screenPositions, pos, norms, uvs, rasterizer);
            break;
    }
    delete rasterizer;
}

void Close2GL::drawNoShading(vector<unsigned int> ids, vector<vec3> pos, vector<vec2> uvs, Rasterizer* rasterizer) {
    for(int i=0; i+2 < ids.size(); i+=3) {
        vector<vec3> vertices = {
            pos[ids[i]],
            pos[ids[i+1]],
            pos[ids[i+2]]
        };
        rasterizer->rasterize(vertices, color);
    }
}

void Close2GL::drawGouraud(vector<unsigned int> ids, vector<vec3> pos, vector<vec3> worldPos, vector<vec3> norms, vector<vec2> uvs, Rasterizer* rasterizer){
    Shader shader(cameraPosition + vec3(2.0, 2.0, 2.0), cameraPosition, color);
    for(int i=0; i+2 < ids.size(); i+=3) {
        vector<vec4> vertices = {
            vec4(pos[ids[i  ]], WValues.at(ids[i  ])),
            vec4(pos[ids[i+1]], WValues.at(ids[i+1])),
            vec4(pos[ids[i+2]], WValues.at(ids[i+2]))
        };
        vector<vec3> colors = {
            shader.applyPhongLightingModel(worldPos[ids[i  ]], norms[ids[i  ]]),
            shader.applyPhongLightingModel(worldPos[ids[i+1]], norms[ids[i+1]]),
            shader.applyPhongLightingModel(worldPos[ids[i+2]], norms[ids[i+2]])
        };
        rasterizer->rasterize(vertices, colors);
    }
}

unsigned char* Close2GL::getRenderedData() {
    return colorBuffer.data();
}

vector<vec3> Close2GL::transform(vector<vec3> array, mat4 transformation, bool perspectiveDivide){
    int count = array.size();
    if(perspectiveDivide){
        WValues.clear();
        WValues.reserve(count);
    }
    for (int i = 0; i < count; i++) {
        vec4 newValue(array[i], 1.0f);
        newValue = transformation * newValue;
        if(perspectiveDivide){
            newValue.x /= newValue.w;
            newValue.y /= newValue.w;
            newValue.z /= newValue.w;
            WValues.push_back(newValue.w);
        }
        array[i] = newValue;
    }
    return array;
}

vector<unsigned int> Close2GL::viewFrustumCull(vector<unsigned int> ids, vector<vec3> pos){
    int indexCount = ids.size();
    for (int i=0; i+2<indexCount; i+=3) {
        vec3 vertex1 = pos[ids[i]];
        vec3 vertex2 = pos[ids[i+1]];
        vec3 vertex3 = pos[ids[i+2]];

        if (
            vertex1.x < -1 || vertex1.x > 1 ||
            vertex2.x < -1 || vertex2.x > 1 ||
            vertex3.x < -1 || vertex3.x > 1 || 
            vertex1.y < -1 || vertex1.y > 1 ||
            vertex2.y < -1 || vertex2.y > 1 ||
            vertex3.y < -1 || vertex3.y > 1 || 
            vertex1.z < -1 || vertex1.z > 1 ||
            vertex2.z < -1 || vertex2.z > 1 ||
            vertex3.z < -1 || vertex3.z > 1 ) {

            ids.erase(ids.begin()+i, ids.begin()+i+3);
            i-=3;
            indexCount-=3;

        }
    }
    return ids;
}

vector<unsigned int> Close2GL::backfaceCull(vector<unsigned int> ids, vector<vec3> pos){
    int indexCount = ids.size();
    for (int i=0; i+2<indexCount; i+=3) {
        vec3 vertex1 = pos[ids[i]];
        vec3 vertex2 = pos[ids[i+1]];
        vec3 vertex3 = pos[ids[i+2]];

        int pair1 = vertex1.x*vertex2.y - vertex2.x*vertex1.y;
        int pair2 = vertex2.x*vertex3.y - vertex3.x*vertex2.y;
        int pair3 = vertex3.x*vertex1.y - vertex1.x*vertex3.y;

        float a = (pair1 + pair2 + pair3)/2.0f;

        bool isCW = frontFace == CW;
        if (((a > 0) && (isCW)) || ((a < 0) && !isCW)) {

            ids.erase(ids.begin()+i, ids.begin()+i+3);
            i-=3;
            indexCount-=3;

        }
    }
    return ids;
}

float Close2GL::calculateHorizontalFOV(){
    return 2.0f * atan(tan(verticalFOV/2.0f)/height*width);
}

void Close2GL::updateView(){
    vec3 n = normalize(-cameraDirection);
    vec3 u = cross(normalize(cameraUp), n);
    vec3 v = cross(n,u);
    view = mat4(0.0f);

    view[0][0] = u.x;
    view[1][0] = u.y;
    view[2][0] = u.z;
    view[3][0] = dot(-cameraPosition, u);

    view[0][1] = v.x;
    view[1][1] = v.y;
    view[2][1] = v.z;
    view[3][1] = dot(-cameraPosition, v);

    view[0][2] = n.x;
    view[1][2] = n.y;
    view[2][2] = n.z;
    view[3][2] = dot(-cameraPosition, n);

    view[3][3] = 1;
}

void Close2GL::updateProjection(){
    float widthNear = 2 * tan(horizontalFOV/2) * (near);
    float heightNear = 2 * tan(verticalFOV/2) * (near);
    float frustumLength = far - near;
    projection = mat4(0.0);

    projection[0][0] = 2 * near / widthNear;
    projection[1][1] = 2 * near / heightNear;
    projection[2][2] = -(far + near)/ frustumLength;
    projection[3][2] = - 2 * far * near / frustumLength;
    projection[2][3] = -1;
}