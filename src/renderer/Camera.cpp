#include "Camera.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Model.h"
#include "Renderer.h"

using glm::normalize;
using glm::cross;
using glm::radians;
using glm::rotate;

#define MAX_ROTATION  radians( 360.0f)
#define MIN_ROTATION  radians(-360.0f)

float Camera::movementUnits;

vec3 Camera::position; 
vec3 Camera::direction; 
vec3 Camera::up; 

float Camera::yaw   = radians(90.0f);
float Camera::pitch = 0.0f;
float Camera::roll = 0.0f;

vec3 Camera::focusPoint; 
vec3 Camera::firstPosition; 
bool Camera::shouldFocusCenter = false;

bool Camera::hasChanged = false;

Camera::Camera() {
    position  = vec3(0.0f, 0.0f,  0.0f);
    direction = vec3(0.0f, 0.0f, -1.0f);
    up        = vec3(0.0f, 1.0f,  0.0f);
    movementUnits = DEFAULT_MOVEMENT_UNITS;
    setFocusPoint(Model::center, Model::width, Model::length);
}

void Camera::setFocusPoint(vec3 focusPoint, float width, float length) {
    Camera::focusPoint = focusPoint;
    Camera::position = focusPoint;
    position.z += (width/2)/tan(Renderer::verticalFieldOfView/2.0) + length;
    firstPosition = position;
    focus();
    hasChanged = true;
}

void Camera::updateDirection(float xMovement, float yMovement) {
    if(!shouldFocusCenter){
        yaw     -= xMovement * 0.05;
        pitch   -= yMovement * 0.05;
        normalizeRotation();
        vec3 newDirection;
        newDirection.x = cos(yaw) * cos(pitch);
        newDirection.y = sin(pitch);
        newDirection.z = -(sin(yaw) * cos(pitch));
        direction = newDirection;
        hasChanged = true;
    }
}

void Camera::setRoll(float newRoll) {
    roll = radians(newRoll);
    up = rotate(vec3(0.0f, 1.0f, 0.0f), roll, direction);
    hasChanged = true;
}

void Camera::moveFront() {
    position += direction * movementUnits;
    hasChanged = true;
}

void Camera::moveBack() {
    position -= direction * movementUnits;
    hasChanged = true;
}

void Camera::moveRight() {
    if(shouldFocusCenter) focus();
    vec3 right = cross(-direction, up);
    position -= right * movementUnits;
    if(shouldFocusCenter) focus();
    hasChanged = true;
}

void Camera::moveLeft() {
    if(shouldFocusCenter) focus();
    vec3 left = cross(direction, up);
    position -= left * movementUnits;
    if(shouldFocusCenter) focus();
    hasChanged = true;
}

void Camera::moveUp() {
    if(shouldFocusCenter) focus();
    vec3 right = cross(-direction, up);
    vec3 upFromCamera = cross(direction, right);
    position += upFromCamera * movementUnits;
    if(shouldFocusCenter) focus();
    hasChanged = true;
}

void Camera::moveDown() {
    if(shouldFocusCenter) focus();
    vec3 left = cross(direction, up);
    vec3 downFromCamera = cross(direction, left);
    position += downFromCamera * movementUnits;
    if(shouldFocusCenter) focus();
    hasChanged = true;
}

void Camera::center() {
    shouldFocusCenter = not shouldFocusCenter;
    if (shouldFocusCenter) focus();
    hasChanged = true;
}

void Camera::reset() {
    position = firstPosition;
    focus();
    hasChanged = true;
}

void Camera::focus() {
    direction = normalize(focusPoint - position);
    pitch = asin(direction.y);
    yaw = acos(direction.x /cos(pitch));
}

void Camera::normalizeRotation() {
    yaw     = fmin(MAX_ROTATION, fmax(yaw, MIN_ROTATION));
    pitch   = fmin(MAX_ROTATION, fmax(pitch, MIN_ROTATION));
}
