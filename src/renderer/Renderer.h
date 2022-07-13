#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

#include "Model.h"
#include "Camera.h"

using std::string;
using glm::mat4;

#define DEFAULT_ASPECT_RATIO 1920.0f/1080.0f

#define VERTEX_SHADER_LOCATION "../shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_LOCATION "../shaders/fragment_shader.glsl"


class Renderer {
public:
    Renderer(const char*, Camera*);
    void setModel(Model);
    void render(int, int);

private:
    Camera* camera;

    float screenWidth;
    float screenHeight;

    float aspectRatio;

    int shaderProgramID;
    int vertexArrayID;

    Model model;

    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;

    int modelUniformID;
    int viewUniformID;
    int projectionUniformID;
    int colorUniformID;

    int count = 0;

    unsigned int loadShader(string, unsigned int);
    void linkShaderProgram(unsigned int);
    unsigned int createShaderProgram(unsigned int, unsigned int);
    void updateProjectionMatrix();
};
