#include "Window.h"
#include <iostream>
#include <cstdlib>

int Window::width = DEFAULT_WIDTH;
int Window::height = DEFAULT_HEIGHT;

Camera* Window::camera;

float Window::mouseXPosition;
float Window::mouseYPosition;
bool Window::hasMouseMoved = false;
bool Window::cameraIsBeingMoved = false;


Window::Window(Camera* camera) {
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Render", NULL, NULL);
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, Window::cursorPositionCallback);
    glfwSetMouseButtonCallback(window, Window::mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, Window::windowSizeChangedCallback);

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

void Window::cursorPositionCallback(GLFWwindow* w, double xPos, double yPos) {
    if(cameraIsBeingMoved) {
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
}

void Window::mouseButtonCallback(GLFWwindow* w, int button, int action, int modes) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cameraIsBeingMoved = true;
        } else if (action == GLFW_RELEASE) {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cameraIsBeingMoved = false;
            hasMouseMoved = false;
        }
    }
}

void Window::windowSizeChangedCallback(GLFWwindow* w, int width, int height) {
    Window::width = width;
    Window::height = height;
}

