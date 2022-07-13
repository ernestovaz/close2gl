#include "RenderingWindow.h"
#include <iostream>
#include <cstdlib>

int RenderingWindow::width = DEFAULT_WIDTH;
int RenderingWindow::height = DEFAULT_HEIGHT;

Camera* RenderingWindow::camera;

float RenderingWindow::mouseXPosition;
float RenderingWindow::mouseYPosition;
bool RenderingWindow::hasMouseMoved = false;
bool RenderingWindow::cameraIsBeingMoved = false;


RenderingWindow::RenderingWindow(Camera* camera) {
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

    glfwSetCursorPosCallback(window, RenderingWindow::cursorPositionCallback);
    glfwSetMouseButtonCallback(window, RenderingWindow::mouseButtonCallback);
    glfwSetScrollCallback(window, RenderingWindow::scrollCallback);
    glfwSetFramebufferSizeCallback(window, RenderingWindow::windowSizeChangedCallback);
    glfwSetKeyCallback(window, RenderingWindow::keyCallback);

    this->camera = camera;
}

RenderingWindow::~RenderingWindow() {
    glfwTerminate();
}

bool RenderingWindow::shouldClose() {
    return glfwWindowShouldClose(window);
}

void RenderingWindow::swapBuffers() {
    lastFrameTime = glfwGetTime();
    glfwSwapBuffers(window);
}

void RenderingWindow::pollEvents() {
    glfwPollEvents();
}

double RenderingWindow::getTimeSinceLastFrame() {
    return glfwGetTime() - lastFrameTime;
}

const char* RenderingWindow::getProcessAddress() {
    return (const char*) glfwGetProcAddress;
}

void RenderingWindow::cursorPositionCallback(GLFWwindow* w, double xPos, double yPos) {
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

void RenderingWindow::mouseButtonCallback(GLFWwindow* w, int button, int action, int modes) {
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

void RenderingWindow::scrollCallback(GLFWwindow* w, double xOffset, double yOffset) {
    camera->changeFOV(yOffset);
}

void RenderingWindow::keyCallback(GLFWwindow*, int key, int scancode, int action, int mods) {
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

void RenderingWindow::windowSizeChangedCallback(GLFWwindow* w, int width, int height) {
    RenderingWindow::width = width;
    RenderingWindow::height = height;
}

