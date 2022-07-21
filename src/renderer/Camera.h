#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::mat4;

#define SENSITIVITY 50.0f
#define DEFAULT_MOVEMENT_UNITS 2.0f

class Camera {
public:
    Camera();

    static void updateDirection(float, float);
    static void setFocusPoint(vec3, float);

    static vec3 position;
    static vec3 direction;
    static vec3 up;
    static float movementUnits;

    static void moveFront();
    static void moveBack();
    static void moveRight();
    static void moveLeft();
    static void moveUp();
    static void moveDown();
    static void center();
    static void focus();
    static void reset();

private:
    static bool shouldFocusCenter;
    static vec3 focusPoint;
    static vec3 firstPosition;
};
