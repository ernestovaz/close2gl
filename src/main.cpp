#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"

#define TARGET_FRAME_RATE 60
#define TIME_PER_FRAME 1.0/TARGET_FRAME_RATE

int main(void) {
    Camera camera;
    Window window;
    Renderer renderer(window.getProcessAddress(), camera.getViewMatrix());

    double elapsedTime;
    while (!window.shouldClose()) {
        elapsedTime = window.getTimeSinceLastFrame();
        window.pollEvents();
        if (elapsedTime >= TIME_PER_FRAME) {
            renderer.render();
            window.swapBuffers();
        }
    }
    return 0;
}
