#pragma once

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "ShadingMethod.h"
#include "Model.h"

using std::string;
using glm::mat4;

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
    unsigned int openGLProgram;
    unsigned int close2GLProgram;


    unsigned int openGLVAO;
    unsigned int openGLEBO;
    unsigned int close2GLVAO;
    unsigned int close2GLEBO;
    unsigned int close2GLVBO;

    int openGLModelUniform;
    int openGLViewUniform;
    int openGLProjectionUniform;
    int openGLColorUniform;
    int openGLShadingSubroutine;

    int close2GLColorUniform;

    void openGLRender();
    void close2GLRender();

    unsigned int createOpenGLShaderProgram();
    unsigned int createClose2GLShaderProgram();
    void linkShaderProgram(unsigned int);
    unsigned int createShaderProgram(unsigned int, unsigned int);
    unsigned int loadShader(string, unsigned int);

    void initializeOpenGL();
    void initializeClose2GL();

    unsigned int createOpenGLVAO();
    void updateClose2GLVAO();

    void initializeOpenGLShadingSubroutines();

    void setShadingMethod(ShadingMethod);
    mat4 openGLProjectionMatrix();
    mat4 openGLViewMatrix();
};
