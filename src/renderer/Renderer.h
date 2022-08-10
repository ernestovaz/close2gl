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

enum class RenderingPrimitive { POINTS, LINES, TRIANGLES };
enum class RenderingAPI { OpenGL, Close2GL };

// Default settings values
#define DEFAULT_RENDERING_PRIMITIVE         RenderingPrimitive::TRIANGLES
#define DEFAULT_RENDERING_COLOR             vec3(0.340f, 0.514f, 0.877f)
#define DEFAULT_FIELD_OF_VIEW      45
#define DEFAULT_NEAR_PLANE                  0.01f
#define DEFAULT_FAR_PLANE                   200.0f
#define DEFAULT_FACE_ORIENTATION_REVERSE    false
#define DEFAULT_CULLING                     false

class Renderer {
public:
    Renderer();
    void render();

    static int currentShadingMethod;
    static RenderingAPI currentAPI;

    // Settings
    static RenderingPrimitive renderingPrimitive;
    static vec3 renderingColor;
    static vec3 backgroundColor;
    static float verticalFieldOfView;
    static float horizontalFieldOfView;
    static bool fieldOfViewIsAsymmetric;
    static float nearPlane;
    static float farPlane;
    static bool cullingEnabled;
    static int reverseFaceOrientation;

private:
    unsigned int openGLProgram;
    unsigned int close2GLProgram;

    unsigned int openGLVAO;
    unsigned int openGLEBO;
    unsigned int close2GLVAO;
    unsigned int close2GLTexture;
    
    unsigned char* colorBuffer;

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
