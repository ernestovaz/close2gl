#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

using std::string;

#define DEFAULT_FIELD_OF_VIEW 3.141592f/3.0f 
#define DEFAULT_NEAR_PLANE -0.1f
#define DEFAULT_FAR_PLANE -400.0f

class Renderer {
public:
    Renderer();
    ~Renderer();
    void render();

private:
    float screenWidth;
    float screenHeight;
    float fieldOfView;

    int shaderProgramID;
    int vertexArrayID;

    GLFWwindow* window;

    unsigned int loadShader(string pathToShader, unsigned int shaderType);
    void linkShaderProgram(unsigned int shaderProgramID);
    unsigned int createShaderProgram(
            unsigned int vertexShaderID, 
            unsigned int fragmentShaderID);
};
