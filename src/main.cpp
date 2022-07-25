#include <iostream>
#include <thread>

#include "renderer/Renderer.h"
#include "renderer/Camera.h"
#include "renderer/Model.h"
#include "application/Window.h"

#define FALLBACK_MODEL_PATH "../data/shark.fbx"

// SYNTAX: <executable> [PATH_TO_MODEL]

int main(int argc, char **argv) {
    const char* modelFilePath = argc == 2 ? 
        argv[1] : FALLBACK_MODEL_PATH;
    Model::loadFromFile(modelFilePath);

    Window window;
    Camera camera;
    Renderer renderer;

    while (!window.shouldClose()) {
        window.pollEvents();
        renderer.render();
        window.swapBuffers();
    }
    return 0;
}
