#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>

using glm::lookAt;

Camera::Camera() {
  position  = vec3(0.0f, 0.0f,  10.0f);
  direction = vec3(0.0f, 0.0f, -10.0f);
  up        = vec3(0.0f, 1.0f,  0.0f);
}

mat4 Camera::getViewMatrix() {
  return lookAt(position, position+direction, up);
}
