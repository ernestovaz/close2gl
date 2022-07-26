#pragma once

#include <vector>

#include <glm/glm.hpp>

using std::vector;

using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Close2GL {
    vector<vec3> transformPositions(vector<vec3> positions, mat4 modelViewProjection);
    mat4 viewMatrix(vec3 cameraPosition, vec3 cameraDirection, vec3 cameraUp);
    mat4 projectionMatrix(float FOVx, float FOVy, float nearPlane, float farPlane);
    mat4 viewMatrix(vec3 cameraPosition, vec3 cameraDirection, vec3 cameraUp);
    float horizontalFieldOfView(float FOVy, float screenWidth, float screenHeight);
}

