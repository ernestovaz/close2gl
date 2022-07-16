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
    glfwSetScrollCallback(window, Window::scrollCallback);
    glfwSetFramebufferSizeCallback(window, Window::windowSizeChangedCallback);
    glfwSetKeyCallback(window, Window::keyCallback);

    this->camera = camera;

    this->settingsToolbox = new SettingsToolbox(window);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
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

void Window::cursorPositionCallback(GLFWwindow* w, double xPos, double yPos) {
    if(cameraIsBeingMoved) {
        if(!hasMouseMoved) {
            glfwSetCursorPos(w, width/2.0f, height/2.0f);
            hasMouseMoved = true;
        } else {
            float normalizedX = (xPos - width / 2.0f ) / width;
            float normalizedY = (yPos - height / 2.0f ) / height;
            camera->updateDirection(normalizedX, normalizedY);
            glfwSetCursorPos(w, width/2.0f, height/2.0f);
            mouseXPosition = xPos;
            mouseYPosition = yPos;
        }
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

void Window::scrollCallback(GLFWwindow* w, double xOffset, double yOffset) {
    camera->changeFOV(yOffset);
}

void Window::keyCallback(GLFWwindow*, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch(key){
            case GLFW_KEY_W:
                camera->moveFront();
                break;
            case GLFW_KEY_S:
                camera->moveBack();
                break;
            case GLFW_KEY_A:
                camera->moveLeft();
                break;
            case GLFW_KEY_D:
                camera->moveRight();
                break;
            case GLFW_KEY_PAGE_UP:
                camera->moveUp();
                break;
            case GLFW_KEY_PAGE_DOWN:
                camera->moveDown();
                break;
            case GLFW_KEY_C:
                camera->center();
                break;
            case GLFW_KEY_I:
                camera->reset();
                break;
            case GLFW_KEY_N:
                camera->cycleNearPlane();
                break;
            case GLFW_KEY_F:
                camera->cycleFarPlane();
                break;
            case GLFW_KEY_R:
                camera->randomizeColor();
                break;
            case GLFW_KEY_P:
                camera->cyclePrimitives();
                break;
            case GLFW_KEY_B:
                camera->cycleCulling();
                break;
            case GLFW_KEY_O:
                camera->cycleOrientation();
                break;
        }
    }
}

void Window::windowSizeChangedCallback(GLFWwindow* w, int width, int height) {
    Window::width = width;
    Window::height = height;
}

