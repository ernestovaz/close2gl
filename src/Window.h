#pragma once

#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();
    bool shouldClose();
    void swapBuffers();
    void pollEvents();
    double getTimeSinceLastFrame();
    const char* getProcessAddress();

private:
    GLFWwindow* window;
    double lastFrameTime;

};
