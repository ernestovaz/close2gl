#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "ShadingMethod.h"
#include "Model.h"

using std::string;

#define OPENGL_SHADER_LOCATION "../src/openGL/shaders/"
#define CLOSE2GL_SHADER_LOCATION "../src/close2GL/shaders/"

enum class RenderingAPI { OpenGL, Close2GL };

class Renderer {
public:
    Renderer();
    void render();

    static int currentShadingMethod;
    static RenderingAPI currentAPI;

private:
    unsigned int openGLProgramID;
    unsigned int close2GLProgramID;
    unsigned int shaderProgramID;

    unsigned int vertexArrayID;

    int modelUniformID;
    int viewUniformID;
    int projectionUniformID;
    int colorUniformID;
    int shadingSubroutineID;

    unsigned int createOpenGLShaderProgram();
    unsigned int createClose2GLShaderProgram();
    void linkShaderProgram(unsigned int);
    unsigned int createShaderProgram(unsigned int, unsigned int);
    unsigned int loadShader(string, unsigned int);

    void initializeShadingSubroutines();
    void initializeModelArrays();

    void setShadingMethod(ShadingMethod);
    void initializeModelMatrix();
    void updateProjectionMatrix();
    void updateViewMatrix();
    void checkAndUpdateProjectionMatrix();
    void checkAndUpdateViewMatrix();
};
