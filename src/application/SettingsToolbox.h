#pragma once

#include<GLFW/glfw3.h>

#include "../renderer/Camera.h"

class SettingsToolbox {
public:
    SettingsToolbox(GLFWwindow*, Camera*);
    ~SettingsToolbox();
    void render();
    bool isHandlingMouse();
    bool quitButtonClicked = false;
};
