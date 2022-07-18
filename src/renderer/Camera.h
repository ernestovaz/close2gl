#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::mat4;

#define SENSITIVITY 50.0f
#define MOVEMENT_SPEED 0.5f

class Camera {
public:
    Camera();

    static vec3 position;
    static vec3 direction;
    static vec3 up;

    void updateDirection(float, float);
    void setFocus(vec3, float);
    void changeFOV(float);
    void moveFront();
    void moveBack();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void center();
    void reset();
    void focus();

private:
    bool shouldFocusCenter;
    vec3 focusPoint;
    vec3 firstPosition;
};
