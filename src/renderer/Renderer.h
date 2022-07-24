#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "ShadingMode.h"
#include "Model.h"

using std::string;
using std::vector;

#define VERTEX_SHADER_LOCATION "../shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_LOCATION "../shaders/fragment_shader.glsl"


class Renderer {
public:
    Renderer();
    void setModel(Model);
    void render();

    static void setShadingMode(ShadingMode);
    static vector<ShadingMode> shadingModes;

private:
    unsigned int shaderProgramID;
    unsigned int vertexArrayID;

    Model model;

    int modelUniformID;
    int viewUniformID;
    int projectionUniformID;
    int colorUniformID;
    int shadingSubroutineID;

    unsigned int loadShader(string, unsigned int);
    unsigned int createShaderProgram(unsigned int, unsigned int);
    void linkShaderProgram(unsigned int);

    void initializeShadingFunctions();

    void initializeModelMatrix();
    void updateProjectionMatrix();
    void updateViewMatrix();
    void checkAndUpdateProjectionMatrix();
    void checkAndUpdateViewMatrix();
};
