#include "Close2GL.h"

#include "Rasterizer.h"

#include <cstddef>
#include <algorithm>
#include <iostream>

using std::sort;
using std::swap;

vector<vec3> Close2GL::transformAndPerspectiveDivide(vector<vec3> positions, mat4 modelViewProjection, vector<float>& wValues) {
    int vertexCount = positions.size();
    vector<vec3> transformedVector(vertexCount);
    wValues.reserve(vertexCount);
    for (int i = 0; i < vertexCount; i++) {
        vec4 newPosition(positions[i], 1.0f);
        newPosition = modelViewProjection * newPosition;
        newPosition.x /= newPosition.w;
        newPosition.y /= newPosition.w;
        newPosition.z /= newPosition.w;
        wValues.push_back(newPosition.w);
        transformedVector[i] = newPosition;
    }
    return transformedVector;
}

vector<vec3> Close2GL::transform(vector<vec3> positions, mat4 transformation) {
    int vertexCount = positions.size();
    vector<vec3> transformedVector(vertexCount);
    for (int i = 0; i < vertexCount; i++) {
        vec4 newPosition(positions[i], 1.0f);
        transformedVector[i] = transformation * newPosition;
    }
    return transformedVector;
}

vector<unsigned int> Close2GL::viewFrustumCulling(vector<unsigned int> indices, vector<vec3> positions) { 
    int indexCount = indices.size();
    for (int i=0; i+2<indexCount; i+=3) {
        vec3 vertex1 = positions[indices[i]];
        vec3 vertex2 = positions[indices[i+1]];
        vec3 vertex3 = positions[indices[i+2]];

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

            indices.erase(indices.begin()+i, indices.begin()+i+3);
            i-=3;
            indexCount-=3;

        }
    }

    return indices;
}

vector<unsigned int> Close2GL::backfaceCulling(vector<unsigned int> indices, vector<vec3> positions, bool isCW) { 
    int indexCount = indices.size();
    for (int i=0; i+2<indexCount; i+=3) {
        vec3 vertex1 = positions[indices[i]];
        vec3 vertex2 = positions[indices[i+1]];
        vec3 vertex3 = positions[indices[i+2]];

        int pair1 = vertex1.x*vertex2.y - vertex2.x*vertex1.y;
        int pair2 = vertex2.x*vertex3.y - vertex3.x*vertex2.y;
        int pair3 = vertex3.x*vertex1.y - vertex1.x*vertex3.y;

        float a = (pair1 + pair2 + pair3)/2.0f;

        if (((a > 0) && isCW) || ((a < 0) && !isCW)) {

            indices.erase(indices.begin()+i, indices.begin()+i+3);
            i-=3;
            indexCount-=3;

        }
    }

    return indices;
}

mat4 Close2GL::projectionMatrix(float FOVx, float FOVy, float nearPlane, float farPlane) {

    float nearPlaneWidth = 2 * tan(FOVx/2) * (nearPlane);
    float nearPlaneHeight = 2 * tan(FOVy/2) * (nearPlane);
    float frustumLength = farPlane - nearPlane;

    mat4 projection = mat4(0.0);

    projection[0][0] = 2 * nearPlane / nearPlaneWidth;
    projection[1][1] = 2 * nearPlane / nearPlaneHeight;
    projection[2][2] = -(farPlane + nearPlane)/ frustumLength;
    projection[3][2] = - 2 * farPlane * nearPlane / frustumLength;
    projection[2][3] = -1;

    return projection;
}

mat4 Close2GL::viewMatrix(vec3 cameraPosition, vec3 cameraDirection, vec3 cameraUp) {
    vec3 n = normalize(-cameraDirection);
    vec3 u = cross(normalize(cameraUp), n);
    vec3 v = cross(n,u);
    mat4 projection(0.0);

    projection[0][0] = u.x;
    projection[1][0] = u.y;
    projection[2][0] = u.z;
    projection[3][0] = dot(-cameraPosition, u);

    projection[0][1] = v.x;
    projection[1][1] = v.y;
    projection[2][1] = v.z;
    projection[3][1] = dot(-cameraPosition, v);

    projection[0][2] = n.x;
    projection[1][2] = n.y;
    projection[2][2] = n.z;
    projection[3][2] = dot(-cameraPosition, n);

    projection[3][3] = 1;

    return projection;
}

mat4 Close2GL::viewportMatrix(int left, int top, int right, int bottom) {
    mat4 viewport(0.0);
    
    // Scaling
    viewport[0][0] = (right - left)/2.0f;
    viewport[1][1] = (bottom - top)/2.0f;   // negative value, flips mapping
    viewport[2][2] = 1;
    viewport[3][3] = 1;

// Translation
    viewport[3][0] = (right + left)/2.0f;
    viewport[3][1] = (bottom + top)/2.0f;
    
    return viewport;
}

float Close2GL::horizontalFieldOfView(float FOVy, float screenWidth, float screenHeight) {
    return 2.0f * atan(tan(FOVy/2.0f)/screenHeight*screenWidth);
}

void Close2GL::rasterize(
    ColorBuffer& buffer, 
    vec3 color, 
    vector<unsigned int> indices, 
    vector<vec3> positions, 
    int primitive
) {
    color *= 255.0f;
    int indexCount = indices.size();
    DepthBuffer empty;
    Rasterizer rasterizer(buffer, empty);
    for (int i=0; i+2 < indexCount; i+=3) {
        vector<vec3> vertices = {
            positions[indices[i]],
            positions[indices[i+1]],
            positions[indices[i+2]]
        };
        switch(primitive) { 
            case TRIANGLES:
                rasterizer.rasterizeTriangle(vertices, color);
                break;
            case LINES:
                rasterizer.rasterizeLines(vertices, color);
                break;
            case POINTS:
                rasterizer.rasterizePoints(vertices, color);
                break;
        }
    }
}

void Close2GL::rasterize(
    ColorBuffer& colorBuffer, 
    DepthBuffer& depthBuffer,
    vector<unsigned int> indices, 
    vector<vec3> positions, 
    vector<vec3> cameraPositions,
    vector<vec3> normals, 
    vector<float> wValues,
    int primitive, 
    Shader shader
) {
    Rasterizer rasterizer(colorBuffer, depthBuffer);

    int indexCount = indices.size();
    for (int i=0; i+2 < indexCount; i+=3) {
        vector<vec3> vertices = {
            positions[indices[i]],
            positions[indices[i+1]],
            positions[indices[i+2]]
        };
        vector<vec3> vertexNormals = {
            normals[indices[i]],
            normals[indices[i+1]],
            normals[indices[i+2]]
        };
        vector<vec3> colors = {
            shader.applyPhongLightingModel(cameraPositions[indices[i]], vertexNormals[0]),
            shader.applyPhongLightingModel(cameraPositions[indices[i+1]], vertexNormals[1]),
            shader.applyPhongLightingModel(cameraPositions[indices[i+2]], vertexNormals[2])
        };
        vector<float> currentWValues = {
            wValues.at(indices[i]),
            wValues.at(indices[i+1]),
            wValues.at(indices[i+2])
        };
        switch(primitive) { 
            case TRIANGLES:
                break;
            case LINES:
                rasterizer.rasterizeLines(vertices, colors, currentWValues);
                break;
            case POINTS:
                rasterizer.rasterizePoints(vertices, colors);
                break;
        }
    }
}