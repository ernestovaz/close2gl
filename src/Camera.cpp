#include "Camera.h"

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#define SENSITIVITY 0.05f

using std::max;
using std::min;

using glm::lookAt;
using glm::normalize;
using glm::radians;

Camera::Camera() {
    position  = vec3(0.0f, 0.0f,  3.0f);
    direction = vec3(0.0f, 0.0f, -1.0f);
    up        = vec3(0.0f, 1.0f,  0.0f);

    yaw   = -90.0f;
    pitch = 0.0f;
}

mat4 Camera::getViewMatrix() {
    return lookAt(position, position+direction, up);
}

void Camera::updateDirection(double x, double y) {
    x*= SENSITIVITY;
    y*= SENSITIVITY;
    yaw += x; 
    pitch += y;

    pitch = max(-89.0f, min(89.0f, pitch));

    vec3 newDirection;
    newDirection.x = cos(radians(yaw)) * cos(radians(pitch));
    newDirection.y = sin(radians(pitch));
    newDirection.z = sin(radians(yaw)) * cos(radians(pitch));

    direction = normalize(newDirection);
}
