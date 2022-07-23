#pragma once

#include <glm/vec3.hpp>
#include <string>

#include "Model.h"

using std::string;

#define VERTEX_SHADER_LOCATION "../shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_LOCATION "../shaders/fragment_shader.glsl"

class Renderer {
public:
    Renderer();
    void setModel(Model);
    void render();

private:
    int shaderProgramID;
    int vertexArrayID;

    Model model;

    int modelUniformID;
    int viewUniformID;
    int projectionUniformID;
    int colorUniformID;

    float lastScreenWidth;
    float lastScreenHeight;
    vec3 lastCameraPosition = vec3(0.0f);
    vec3 lastCameraDirection = vec3(0.0f);

    unsigned int loadShader(string, unsigned int);
    void linkShaderProgram(unsigned int);
    unsigned int createShaderProgram(unsigned int, unsigned int);

    void checkAndUpdateProjectionMatrix();
    void checkAndUpdateViewMatrix();
    void updateProjectionMatrix();
    void updateViewMatrix();
};
