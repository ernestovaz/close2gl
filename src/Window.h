#pragma once

#include "Camera.h"

#include <GLFW/glfw3.h>

class Window {
public:
    Window(Camera* camera);
    ~Window();
    bool shouldClose();
    void swapBuffers();
    void pollEvents();
    double getTimeSinceLastFrame();
    const char* getProcessAddress();
    void updateCameraDirection(Camera* camera);

    static void cursorPositionCallback(GLFWwindow*, double, double);

private:
    GLFWwindow* window;
    int width;
    int height;

    double lastFrameTime;

    static bool hasMouseMoved;

    static float mouseXPosition;
    static float mouseYPosition;
    static Camera* camera;

};
