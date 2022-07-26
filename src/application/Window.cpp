#include "Window.h"

#include <iostream>
#include <cstdlib>

#include "../renderer/Camera.h"

GLFWwindow* Window::window;

int Window::width = DEFAULT_WIDTH;
int Window::height = DEFAULT_HEIGHT;
bool Window::hasSizeChanged = false;

float Window::lastMouseXPosition;
float Window::lastMouseYPosition;
bool Window::hasMouseMoved = false;
bool Window::exitKeyWasPressed = false;
bool Window::cameraIsBeingMoved = false;

SettingsToolbox* Window::settingsToolbox;


Window::Window() {
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Render", NULL, NULL);
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwSwapInterval(0);
    glfwSetCursorPosCallback(window, Window::cursorPositionCallback);
    glfwSetMouseButtonCallback(window, Window::mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, Window::windowSizeChangedCallback);
    glfwSetKeyCallback(window, Window::keyCallback);

    this->settingsToolbox = new SettingsToolbox(window);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window)
        || settingsToolbox->quitButtonClicked
        || exitKeyWasPressed;
}

void Window::swapBuffers() {
    settingsToolbox->render();
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

void Window::cursorPositionCallback(GLFWwindow* w, double xPosition, double yPosition) {
    if(cameraIsBeingMoved) {
        if(!hasMouseMoved) {
            hasMouseMoved = true;
        } else {
            float xMovement = xPosition - lastMouseXPosition;
            float yMovement = yPosition - lastMouseYPosition;
            Camera::updateDirection(xMovement*0.002, yMovement*0.002);
        }
        lastMouseXPosition = xPosition;
        lastMouseYPosition = yPosition;
    }
}

void Window::mouseButtonCallback(GLFWwindow* w, int button, int action, int modes) {
    if (!settingsToolbox->isHandlingMouse() && !Camera::shouldFocusCenter && button == GLFW_MOUSE_BUTTON_LEFT) {
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

void Window::keyCallback(GLFWwindow*, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch(key){
            case GLFW_KEY_W:
                Camera::moveFront();
                break;
            case GLFW_KEY_S:
                Camera::moveBack();
                break;
            case GLFW_KEY_A:
                Camera::moveLeft();
                break;
            case GLFW_KEY_D:
                Camera::moveRight();
                break;
            case GLFW_KEY_PAGE_UP:
                Camera::moveUp();
                break;
            case GLFW_KEY_PAGE_DOWN:
                Camera::moveDown();
                break;
            case GLFW_KEY_ESCAPE:
                exitKeyWasPressed = true;
                break;
        }
    }
}


void Window::windowSizeChangedCallback(GLFWwindow* w, int width, int height) {
    Window::width = width;
    Window::height = height;
    Window::hasSizeChanged = true;
}

