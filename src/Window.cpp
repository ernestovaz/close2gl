#include "Window.h"
#include <iostream>
#include <cstdlib>

Camera* Window::camera;
float Window::mouseXPosition;
float Window::mouseYPosition;
bool Window::hasMouseMoved = false;


Window::Window(Camera* camera) {
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    window = glfwCreateWindow(1920, 1080, "HELLO WORLD", NULL, NULL);
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, Window::cursorPositionCallback);

    glfwGetWindowSize(window, &width, &height);
    this->mouseXPosition = width/2.0;
    this->mouseYPosition = height/2.0;

    this->camera = camera;
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    lastFrameTime = glfwGetTime();
    glfwSwapBuffers(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

double Window::getTimeSinceLastFrame() {
    return glfwGetTime() - lastFrameTime;
}

const char* Window::getProcessAddress() {
    return (const char*) glfwGetProcAddress;
}

void Window::updateCameraDirection(Camera* camera) {
    camera->updateDirection(mouseXPosition, mouseYPosition);
}

void Window::cursorPositionCallback(GLFWwindow* w, double xPos, double yPos) {
    if(!hasMouseMoved) {
        mouseXPosition = xPos;
        mouseYPosition = yPos;
        hasMouseMoved = true;
    }
    float xOffset = xPos - mouseXPosition;
    float yOffset = mouseYPosition - yPos;
    camera->updateDirection(xOffset, yOffset);
    mouseXPosition = xPos;
    mouseYPosition = yPos;
}



