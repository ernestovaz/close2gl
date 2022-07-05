#include "Camera.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

using std::max;
using std::min;

using glm::lookAt;
using glm::normalize;
using glm::cross;
using glm::radians;

Camera::Camera() {
    position  = vec3(0.0f, 0.0f,  0.0f);
    direction = vec3(0.0f, 0.0f, -1.0f);
    up        = vec3(0.0f, 1.0f,  0.0f);

    fieldOfView = DEFAULT_FIELD_OF_VIEW;
    farPlane = DEFAULT_FAR_PLANE;
    nearPlane = DEFAULT_NEAR_PLANE;

    color = vec3(0.89f, 0.427f, 0.819f);

    primitive = 1;

    culling = false;
}

void Camera::setFocus(vec3 focus, float boundingBoxSide) {
    this->focusPoint = focus;
    this->position.z = (boundingBoxSide/2)/tan(fieldOfView/2.0);
    this->firstPosition = position;
    this->focus();
}

mat4 Camera::getViewMatrix() {
    return lookAt(position, position+direction, up);
}

float Camera::getFOV() {
    return fieldOfView;
}

float Camera::getNearPlane() {
    return nearPlane;
}

float Camera::getFarPlane() {
    return farPlane;
}

vec3 Camera::getColor() {
    return color;
}

int Camera::getPrimitive() {
    return primitive;
}

bool Camera::getCulling() { 
    return culling;
}

bool Camera::shouldReverseOrientation() {
    return reverseOrientation;
}

void Camera::updateDirection(float x, float y) {
    if(!centerMode){
        vec3 left = cross(direction, up);
        // pitch rotation
        vec3 newDirection = rotate(direction, radians(-y*SENSITIVITY), left);
        // lock pitch into 85 degrees
        if(angle(newDirection, up) > radians(5.0f) 
                && angle(newDirection,-up) > radians(5.0f)) {
            direction = newDirection;
        }
        // yaw rotation
        direction = rotate(direction, radians(-x*SENSITIVITY), up);
    }
}

void Camera::changeFOV(float offset) {
    fieldOfView = max(min(fieldOfView - offset, 200.0f), 1.0f); 
}

void Camera::moveFront() {
    position += direction * MOVEMENT_SPEED;
}

void Camera::moveBack() {
    position -= direction * MOVEMENT_SPEED;
}

void Camera::moveRight() {
    if(centerMode) focus();
    vec3 right = cross(-direction, up);
    position -= right * MOVEMENT_SPEED;
    if(centerMode) focus();
}

void Camera::moveLeft() {
    if(centerMode) focus();
    vec3 left = cross(direction, up);
    position -= left * MOVEMENT_SPEED;
    if(centerMode) focus();

}

void Camera::moveUp() {
    if(centerMode) focus();
    vec3 right = cross(-direction, up);
    vec3 upFromCamera = cross(direction, right);
    position += upFromCamera * MOVEMENT_SPEED;
    if(centerMode) focus();
}

void Camera::moveDown() {
    if(centerMode) focus();
    vec3 left = cross(direction, up);
    vec3 downFromCamera = cross(direction, left);
    position += downFromCamera * MOVEMENT_SPEED;
    if(centerMode) focus();
}

void Camera::center() {
    focus();
    centerMode = not centerMode;
}

void Camera::reset() {
    position = firstPosition;
    focus();
}

void Camera::focus() {
    direction = normalize(focusPoint - position);
}

void Camera::cycleNearPlane() {
    nearPlane *= 1.5;
    if(nearPlane > 20) {
        nearPlane = DEFAULT_NEAR_PLANE;
    }
}

void Camera::cycleFarPlane() {
    farPlane /= 2;
    if(farPlane < 1) {
        farPlane = DEFAULT_FAR_PLANE;
    }
}

void Camera::cyclePrimitives() {
    primitive > 3 ? primitive = 1 : primitive ++;
}

void Camera::cycleCulling() {
    culling = not culling;
}

void Camera::cycleOrientation() {
    reverseOrientation = not reverseOrientation;
}

void Camera::randomizeColor() {
    srand((unsigned) time(NULL));
    color.x = (rand() % 100)/100.0;
    color.y = (rand() % 100)/100.0;
    color.z = (rand() % 100)/100.0;
}
