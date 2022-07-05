#include <iostream>

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"

#define TARGET_FRAME_RATE 60
#define TIME_PER_FRAME 1.0/TARGET_FRAME_RATE
#define FALLBACK_MODEL_PATH "../data/shark.fbx"

// SYNTAX: <executable> [PATH_TO_MODEL]

int main(int argc, char **argv) {
    const char* modelFilePath = argc == 2 ? 
        argv[1] : FALLBACK_MODEL_PATH;

    Camera camera;
    Window window(&camera);
    Renderer renderer(window.getProcessAddress(), &camera);
    Model model(modelFilePath);
    renderer.setModel(model);
    camera.setFocus(model.getCenter(), model.getBoundingBoxSide());

    double elapsedTime;
    while (!window.shouldClose()) {
        elapsedTime = window.getTimeSinceLastFrame();
        window.pollEvents();
        if (elapsedTime >= TIME_PER_FRAME) {
            renderer.render(
                    window.width, 
                    window.height
            );
            window.swapBuffers();
        }
    }
    return 0;
}
