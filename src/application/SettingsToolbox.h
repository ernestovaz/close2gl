#pragma once

#include<GLFW/glfw3.h>

#include "../renderer/Camera.h"

class SettingsToolbox {
public:
    SettingsToolbox(GLFWwindow*);
    ~SettingsToolbox();
    void render();
    bool isHandlingMouse();
    bool quitButtonClicked = false;
private:
    void renderPrimitivesMenu();
};
