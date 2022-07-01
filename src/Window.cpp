#include "Window.h"

#include <iostream>
#include <cstdlib>

Window::Window() {
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "HELLO WORLD", NULL, NULL);
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
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

