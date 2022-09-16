#include "Close2GLClass.h"
#include "Rasterizer.h"
#include "Shader.h"

#include <iostream>
#include <glm/vec4.hpp>

using std::cout;
using std::endl;
using glm::vec4;

Close2GLClass::Primitive Close2GLClass::primitive = TRIANGLES;
Close2GLClass::Shading Close2GLClass::shading = NO_SHADING;
Close2GLClass::Orientation Close2GLClass::frontFace = CCW;

vec3 Close2GLClass::color = vec3(0.0f, 0.0f, 0.0f);
bool Close2GLClass::useTexture = false;
bool Close2GLClass::useHorizontalFOV = false;
bool Close2GLClass::performBackfaceCulling = false;

vec3 Close2GLClass::cameraPosition = vec3(0.0f, 0.0f, 0.0f);
vec3 Close2GLClass::cameraDirection = vec3(0.0f, 0.0f,-1.0f);
vec3 Close2GLClass::cameraUp = vec3(0.0f, 1.0f, 0.0f);

float Close2GLClass::verticalFOV = 60;
float Close2GLClass::horizontalFOV = 60;
float Close2GLClass::near = 0.01;
float Close2GLClass::far = 10;

ColorBuffer Close2GLClass::colorBuffer; 
DepthBuffer Close2GLClass::depthBuffer; 

mat4 Close2GLClass::model; 
mat4 Close2GLClass::view; 
mat4 Close2GLClass::projection; 
mat4 Close2GLClass::viewport = mat4(0.0f); 

float Close2GLClass::width;
float Close2GLClass::height;

vector<float> Close2GLClass::WValues;


void Close2GLClass::clear(vec3 color){
    colorBuffer.clear(color);
    depthBuffer.clear();
}

void Close2GLClass::updateViewport(int left, int top, int right, int bottom){
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

void Close2GLClass::draw(
    vector<unsigned int> indices, 
    vector<vec3> positions,
    vector<vec3> normals,
    vector<vec2> uvs
) {
    updateView();
    updateProjection();
    vector<vec3> cameraPositions = transform(positions, view, false);
    vector<vec3> projectedPositions = transform(cameraPositions, projection, true);
    vector<unsigned int> culledIndices = viewFrustumCull(indices, projectedPositions);

    vector<vec3> screenPositions = transform(projectedPositions, viewport, false);
    if(performBackfaceCulling){
        culledIndices = backfaceCull(culledIndices, screenPositions);
    }
    
    vec3 colorToUse = color * 255.0f; //TODO:fix this
    int count = culledIndices.size();
    Rasterizer rasterizer(colorBuffer, depthBuffer);
    if(shading == NO_SHADING){
        for(int i=0; i+2 < count; i+=3) {
            vector<vec3> vertices = {
                screenPositions[culledIndices[i]],
                screenPositions[culledIndices[i+1]],
                screenPositions[culledIndices[i+2]]
            };
            switch(primitive){
                case TRIANGLES:
                    rasterizer.rasterizeTriangle(vertices, colorToUse);
                    break;
                case LINES:
                    rasterizer.rasterizeLines(vertices, colorToUse);
                    break;
                case POINTS:
                    rasterizer.rasterizePoints(vertices, colorToUse);
                    break;
            }
        }
    } else {
        Close2GL::Shader shader(cameraPosition + vec3(2.0, 2.0, 2.0), cameraPosition, color);
        for(int i=0; i+2 < count; i+=3) {
            vector<vec3> vertices = {
                screenPositions[culledIndices[i]],
                screenPositions[culledIndices[i+1]],
                screenPositions[culledIndices[i+2]]
            };
            vector<vec3> vertexNormals = {
                normals[culledIndices[i]],
                normals[culledIndices[i+1]],
                normals[culledIndices[i+2]]
            };
            vector<vec3> colors = {
                shader.applyPhongLightingModel(positions[culledIndices[i]], vertexNormals[0]),
                shader.applyPhongLightingModel(positions[culledIndices[i+1]], vertexNormals[1]),
                shader.applyPhongLightingModel(positions[culledIndices[i+2]], vertexNormals[2])
            };
            vector<float> currentW = {
                WValues.at(culledIndices[i]),
                WValues.at(culledIndices[i+1]),
                WValues.at(culledIndices[i+2])
            };
            switch(primitive){
                case TRIANGLES:
                    rasterizer.rasterizeTriangle(vertices, colors, currentW);
                    break;
                case LINES:
                    rasterizer.rasterizeLines(vertices, colors, currentW);
                    break;
                case POINTS:
                    rasterizer.rasterizePoints(vertices, colors);
                    break;
            }
        }
    }
}

unsigned char* Close2GLClass::getRenderedData() {
    return colorBuffer.data();
}

vector<vec3> Close2GLClass::transform(vector<vec3> array, mat4 transformation, bool perspectiveDivide){
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

vector<unsigned int> Close2GLClass::viewFrustumCull(vector<unsigned int> ids, vector<vec3> pos){
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

vector<unsigned int> Close2GLClass::backfaceCull(vector<unsigned int> ids, vector<vec3> pos){
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

float Close2GLClass::calculateHorizontalFOV(){
    return 2.0f * atan(tan(verticalFOV/2.0f)/height*width);
}

void Close2GLClass::updateView(){
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

void Close2GLClass::updateProjection(){
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