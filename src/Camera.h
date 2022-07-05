#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::mat4;

#define SENSITIVITY 0.01f
#define MOVEMENT_SPEED 0.1f

#define DEFAULT_FIELD_OF_VIEW 45
#define DEFAULT_NEAR_PLANE 0.001f
#define DEFAULT_FAR_PLANE 200.0f

class Camera {
public:
    Camera();
    mat4 getViewMatrix();
    vec3 getColor();
    float getFOV();
    float getNearPlane();
    float getFarPlane();
    int getPrimitive();
    bool getCulling();
    bool shouldReverseOrientation();

    void updateDirection(double, double);
    void setFocus(vec3, float);
    void changeFOV(float);
    void moveFront();
    void moveBack();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void center();
    void focus();
    void cycleNearPlane();
    void cycleFarPlane();
    void cyclePrimitives();
    void cycleCulling();
    void cycleOrientation();
    void randomizeColor();

private:
    vec3 position;
    vec3 direction;
    vec3 up;

    float fieldOfView;
    float nearPlane;
    float farPlane;

    vec3 focusPoint;
    bool centerMode = false;
    bool reverseOrientation = false;

    vec3 color;

    int primitive;

    bool culling;

};
