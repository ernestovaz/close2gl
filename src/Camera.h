#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::mat4;

class Camera {
public:
    Camera();
    mat4 getViewMatrix();
private:
    vec3 position;
    vec3 direction;
    vec3 up;
};
