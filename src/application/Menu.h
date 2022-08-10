#pragma once

#include<GLFW/glfw3.h>

#include "../renderer/Camera.h"

class Menu {
public:
    Menu(GLFWwindow*);
    ~Menu();
    void render();
    bool isHandlingMouse();
    bool quitButtonClicked = false;
private:
    void renderPrimitivesMenu();
};
