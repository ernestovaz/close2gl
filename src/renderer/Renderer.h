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

#define TEXTURE_LOCATION "../data/checker.jpg"

enum RenderingPrimitive { 
    POINTS_PRIMITIVE    = 0, 
    LINES_PRIMITIVE     = 1, 
    TRIANGLES_PRIMITIVE = 2 
};
enum RenderingAPI { 
    OpenGL_API  = 0, 
    Close2GL_API= 1 
};
enum TextureFiltering {
    NEAREST_NEIGHBOUR = 0,
    BILINEAR = 1,
    TRILINEAR = 2
};

// Default setting values
#define DEFAULT_RENDERING_PRIMITIVE         RenderingPrimitive::TRIANGLES_PRIMITIVE
#define DEFAULT_TEXTURE_FILTERING           TextureFiltering::NEAREST_NEIGHBOUR
#define DEFAULT_RENDERING_COLOR             vec3(0.340f, 0.514f, 0.877f)
#define DEFAULT_FIELD_OF_VIEW               45
#define DEFAULT_NEAR_PLANE                  0.01f
#define DEFAULT_FAR_PLANE                   200.0f
#define DEFAULT_FACE_ORIENTATION_REVERSE    false
#define DEFAULT_CULLING                     false
#define BACKGROUND_COLOR                    vec3(0.10f, 0.09f, 0.10f) 

class Renderer {
public:
    Renderer();
    ~Renderer();
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
    static bool shouldUseTexture;
    static TextureFiltering textureFiltering;

private:
    unsigned int openGLProgram;
    unsigned int close2GLProgram;
    
    unsigned int texture;

    unsigned int openGLVAO;
    unsigned int openGLEBO;
    unsigned int close2GLVAO;
    unsigned int close2GLTexture;

    int openGLModelUniform;
    int openGLViewUniform;
    int openGLProjectionUniform;
    int openGLColorUniform;
    int openGLCameraPositionUniform;
    int openGLUseTextureUniform;
    int openGLShadingSubroutine;

    int close2GLColorUniform;

    unsigned int createOpenGLShaderProgram();
    unsigned int createOpenGLVAO();
    void loadTexture();
    void initializeOpenGLShadingSubroutines();
    void initializeOpenGL();
    mat4 openGLProjectionMatrix();
    mat4 openGLViewMatrix();
    void openGLResize();
    void openGLRender();

    unsigned int createClose2GLShaderProgram();
    void initializeClose2GL();
    void close2GLResize();
    void close2GLRender();

    void linkShaderProgram(unsigned int);
    unsigned int createShaderProgram(unsigned int, unsigned int);
    unsigned int loadShader(string, unsigned int);
    void setShadingMethod(ShadingMethod);
};
