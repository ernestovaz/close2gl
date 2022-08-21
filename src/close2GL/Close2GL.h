#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "ColorBuffer.h"

using std::vector;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Close2GL {
    const int POINTS = 0;
    const int LINES = 1;
    const int TRIANGLES = 2;

    vector<vec3> transformAndPerspectiveDivide(vector<vec3> positions, mat4 transformation);
    vector<vec3> transform(vector<vec3> positions, mat4 transformation);
    vector<unsigned int> viewFrustumCulling(vector<unsigned int> indices, vector<vec3> positions);
    vector<unsigned int> backfaceCulling(vector<unsigned int> indices, vector<vec3> positions, bool isCW);
    mat4 viewMatrix(vec3 cameraPosition, vec3 cameraDirection, vec3 cameraUp);
    mat4 projectionMatrix(float FOVx, float FOVy, float nearPlane, float farPlane);
    mat4 viewMatrix(vec3 cameraPosition, vec3 cameraDirection, vec3 cameraUp);
    mat4 viewportMatrix(int left, int top, int right, int bottom);
    float horizontalFieldOfView(float FOVy, float screenWidth, float screenHeight);

    void rasterizeNoShading(ColorBuffer& buffer, vec3 color, vector<unsigned int> indices, vector<vec3> positions, int renderingMode);
}