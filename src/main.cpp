#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

int main(void) {
    Renderer renderer;
    while (true) {
        renderer.render();
    }
    return 0;
}
