#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

using std::string;
using glm::mat4;

#define DEFAULT_FIELD_OF_VIEW 3.141592f/3.0f 
#define DEFAULT_ASPECT_RATIO 1920.0f/1080.0f
#define DEFAULT_NEAR_PLANE 0.001f
#define DEFAULT_FAR_PLANE 100.0f

class Renderer {
public:
    Renderer(const char* windowProcessAddress, mat4 viewMatrix);
    void render(mat4 viewMatrix);

private:
    float screenWidth;
    float screenHeight;

    float fieldOfView;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    int shaderProgramID;
    int vertexArrayID;

    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;

    int modelUniformID;
    int viewUniformID;
    int projectionUniformID;

    int count = 0;

    unsigned int loadShader(string pathToShader, unsigned int shaderType);
    void linkShaderProgram(unsigned int shaderProgramID);
    unsigned int createShaderProgram(
            unsigned int vertexShaderID, 
            unsigned int fragmentShaderID);
};
