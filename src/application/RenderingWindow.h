#pragma once

#include "../renderer/Camera.h"

#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

class RenderingWindow {
public:
    RenderingWindow(Camera* camera);
    ~RenderingWindow();

    bool shouldClose();
    void swapBuffers();
    void pollEvents();
    double getTimeSinceLastFrame();
    const char* getProcessAddress();

    static int width;
    static int height;

    static void cursorPositionCallback(GLFWwindow*, double, double);
    static void mouseButtonCallback(GLFWwindow*, int, int , int);
    static void scrollCallback(GLFWwindow* window, double, double);
    static void keyCallback(GLFWwindow*, int, int, int, int);
    static void windowSizeChangedCallback(GLFWwindow*, int, int);

private:
    GLFWwindow* window;

    double lastFrameTime;

    static bool hasMouseMoved;
    static bool cameraIsBeingMoved;
    static float mouseXPosition;
    static float mouseYPosition;
    static Camera* camera;

};
