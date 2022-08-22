#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../application/Window.h"
#include "../close2GL/Close2GL.h"

using std::cerr;
using std::exit;
using std::endl;
using std::ifstream;
using std::ostringstream;

using glm::mat4;
using glm::perspective;
using glm::value_ptr;
using glm::vec3;
using glm::radians;
using glm::degrees;

int Renderer::currentShadingMethod = 0;
RenderingAPI Renderer::currentAPI = RenderingAPI::OpenGL_API;

// Settings
bool Renderer::fieldOfViewIsAsymmetric          = false;
float Renderer::verticalFieldOfView             = DEFAULT_FIELD_OF_VIEW;
float Renderer::horizontalFieldOfView           = DEFAULT_FIELD_OF_VIEW;
float Renderer::farPlane                        = DEFAULT_FAR_PLANE;
float Renderer::nearPlane                       = DEFAULT_NEAR_PLANE;
int Renderer::reverseFaceOrientation            = DEFAULT_FACE_ORIENTATION_REVERSE;
bool Renderer::cullingEnabled                   = DEFAULT_CULLING;
vec3 Renderer::renderingColor                   = DEFAULT_RENDERING_COLOR;
RenderingPrimitive Renderer::renderingPrimitive = DEFAULT_RENDERING_PRIMITIVE;

Renderer::Renderer() {
    if(!gladLoadGLLoader((GLADloadproc) Window::getProcessAddress())) {
        cerr << "ERROR INITIALIZING GLAD" << endl;
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_PROGRAM_POINT_SIZE);


    initializeOpenGL();
    initializeClose2GL();
}

void Renderer::initializeOpenGL() {
    openGLProgram = createOpenGLShaderProgram();

    openGLModelUniform =        glGetUniformLocation(openGLProgram, "model");
    openGLViewUniform =         glGetUniformLocation(openGLProgram, "view");
    openGLProjectionUniform =   glGetUniformLocation(openGLProgram, "projection");
    openGLColorUniform =        glGetUniformLocation(openGLProgram, "color");

    openGLVAO = createOpenGLVAO(); 

    initializeOpenGLShadingSubroutines();
}

void Renderer::initializeClose2GL() {
    unsigned int positionBuffer, textureBuffer;
    close2GLProgram = createClose2GLShaderProgram();
    close2GLColorUniform = glGetUniformLocation(close2GLProgram, "color");

    glGenVertexArrays(1, &close2GLVAO);
    glBindVertexArray(close2GLVAO);

    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    vector<vec2> positions = {
        vec2(-1.0f, -1.0f),
        vec2( 1.0f, -1.0f),
        vec2( 1.0f,  1.0f),
        vec2( 1.0f,  1.0f),
        vec2(-1.0f,  1.0f),
        vec2(-1.0f, -1.0f)
    };
    glBufferData(
            GL_ARRAY_BUFFER, 
            positions.size() * sizeof(vec2), 
            positions.data(), 
            GL_STATIC_DRAW
    );
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    vector<vec2> textures = {
        vec2(0.0f, 0.0f),
        vec2(1.0f, 0.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(0.0f, 1.0f),
        vec2(0.0f, 0.0f)
    };
    glBufferData(
            GL_ARRAY_BUFFER, 
            textures.size() * sizeof(vec2), 
            textures.data(), 
            GL_STATIC_DRAW
    );
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
    glEnableVertexAttribArray(1);
    
    close2GLResize();

    glBindVertexArray(0);
}

void Renderer::close2GLResize() {
    colorBuffer.resize(Window::width, Window::height); 
    depthBuffer.resize(Window::width, Window::height);
    glDeleteTextures(1, &close2GLTexture);
    glGenTextures(1, &close2GLTexture);
    glBindTexture(GL_TEXTURE_2D, close2GLTexture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, Window::width, Window::height);
}

unsigned int Renderer::createOpenGLVAO() {
    unsigned int vertexArrayID, positionBufferID, normalBufferID;

    // set up VAO
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // set up EBO
    glGenBuffers(1, &openGLEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openGLEBO);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, 
            Model::indices.size() * sizeof(unsigned int), 
            Model::indices.data(), 
            GL_STATIC_DRAW
    );

    // set up VBOs
    // positions
    glGenBuffers(1, &positionBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferID);
    glBufferData(
            GL_ARRAY_BUFFER, 
            Model::positions.size() * sizeof(vec3), 
            Model::positions.data(), 
            GL_STATIC_DRAW
    );
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glEnableVertexAttribArray(0);

    // normals
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(
            GL_ARRAY_BUFFER, 
            Model::normals.size() * sizeof(vec3), 
            Model::normals.data(), 
            GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

    glBindVertexArray(0);

    return vertexArrayID;
}

void Renderer::render() {
    if (Window::hasSizeChanged) {
        glViewport(0, 0, Window::width, Window::height);
        Window::hasSizeChanged = false;
        close2GLResize();
    }

    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    if(currentAPI == RenderingAPI::OpenGL_API) openGLRender();
    else close2GLRender();
}

void Renderer::openGLRender() {

    switch(Renderer::renderingPrimitive){
        case RenderingPrimitive::TRIANGLES_PRIMITIVE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case RenderingPrimitive::LINES_PRIMITIVE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case RenderingPrimitive::POINTS_PRIMITIVE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;

    }

    glUseProgram(openGLProgram);
    setShadingMethod(ShadingMethod::methods.at(currentShadingMethod)); 

    mat4 model =        mat4(1.0f);
    mat4 view  =        openGLViewMatrix();
    mat4 projection =   openGLProjectionMatrix();


    glUniformMatrix4fv(openGLModelUniform,1,GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(openGLViewUniform,1,GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(openGLProjectionUniform,1,GL_FALSE, value_ptr(projection));
    glUniform3fv(openGLColorUniform, 1, value_ptr(Renderer::renderingColor));

    if(Renderer::cullingEnabled) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);

    if(Renderer::reverseFaceOrientation) glFrontFace(GL_CW);
    else glFrontFace(GL_CCW);

    glBindVertexArray(openGLVAO);
    glDrawElements(GL_TRIANGLES, Model::indices.size(), GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
}

void Renderer::close2GLRender() {
    glUseProgram(close2GLProgram);
    glDisable(GL_CULL_FACE); // culling should be done in Close2GL
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float FOVy = radians(Renderer::verticalFieldOfView);
    float FOVx;

    if (!Renderer::fieldOfViewIsAsymmetric) {
        //manually calculate horizontal field of view for simmetry
        FOVx = Close2GL::horizontalFieldOfView(FOVy, Window::width, Window::height);
        Renderer::horizontalFieldOfView = degrees(FOVx);
    } else FOVx = radians(Renderer::horizontalFieldOfView);

    mat4 view  = Close2GL::viewMatrix(
            Camera::position,
            Camera::direction,
            Camera::up
    );
    mat4 projection = Close2GL::projectionMatrix(
            FOVx, FOVy, 
            Renderer::nearPlane,
            Renderer::farPlane
    );
    
    vector<vec3> cameraPositions = Close2GL::transform(Model::positions, view);
    vector<vec3> positions = Close2GL::transformAndPerspectiveDivide(cameraPositions, projection);
    vector<unsigned int> indices = Close2GL::viewFrustumCulling(Model::indices, positions);
    mat4 viewport = Close2GL::viewportMatrix(0, 0, Window::width, Window::height);
    positions = Close2GL::transform(positions, viewport);

    if (Renderer::cullingEnabled) {
        indices = Close2GL::backfaceCulling(indices, positions, Renderer::reverseFaceOrientation);
    }
    
    colorBuffer.clear(BACKGROUND_COLOR * 255.0f);
    depthBuffer.clear();
    if(currentShadingMethod == 0) {
        Close2GL::rasterize(colorBuffer, renderingColor, indices, positions, (int)renderingPrimitive);
    } else {
        vector<vec3> normals = Model::normals;
        Shader shader(Camera::position, vec3(1.0f), renderingColor);
        Close2GL::rasterize(colorBuffer, depthBuffer, indices, positions, cameraPositions, normals, (int)renderingPrimitive, shader);
    }

    glBindTexture(GL_TEXTURE_2D, close2GLTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Window::width, Window::height, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer.data());

    glBindVertexArray(close2GLVAO);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

unsigned int Renderer::createOpenGLShaderProgram() {
    unsigned int vertexShaderID, fragmentShaderID;
    string baseShaderLocation = OPENGL_SHADER_LOCATION;

    string vertexShaderLocation = baseShaderLocation + "vertex_shader.glsl";
    vertexShaderID = loadShader(vertexShaderLocation, GL_VERTEX_SHADER);

    string fragmentShaderLocation = baseShaderLocation + "fragment_shader.glsl";
    fragmentShaderID = loadShader(fragmentShaderLocation, GL_FRAGMENT_SHADER);

    unsigned int programID = createShaderProgram(vertexShaderID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    return programID;
}

unsigned int Renderer::createClose2GLShaderProgram() {
    unsigned int vertexShaderID, fragmentShaderID;
    string baseShaderLocation = CLOSE2GL_SHADER_LOCATION;

    string vertexShaderLocation = baseShaderLocation + "vertex_shader.glsl";
    vertexShaderID = loadShader(vertexShaderLocation, GL_VERTEX_SHADER);

    string fragmentShaderLocation = baseShaderLocation + "fragment_shader.glsl";
    fragmentShaderID = loadShader(fragmentShaderLocation, GL_FRAGMENT_SHADER);

    unsigned int programID = createShaderProgram(vertexShaderID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    return programID;
}

unsigned int Renderer::createShaderProgram(
        unsigned int vertexShaderID, 
        unsigned int fragmentShaderID) {
    unsigned int programID;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    linkShaderProgram(programID);
    glUseProgram(programID);

    return programID;
}

void Renderer::linkShaderProgram(unsigned int shaderProgramID) {
    glLinkProgram(shaderProgramID);
    int successful;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &successful);
    if(!successful) {
        char errorLog[512];
        glGetProgramInfoLog(shaderProgramID, 512, NULL, errorLog);
        cerr << "ERROR LINKING SHADER PROGRAM " << shaderProgramID << ": " << errorLog << endl;
        exit(EXIT_FAILURE);
    }
}

unsigned int Renderer::loadShader(string pathToShader, unsigned int shaderType) {
    ifstream fileReader;
    unsigned int shaderID;

    fileReader.open(pathToShader);
    if(fileReader.fail()){
        cerr << "ERROR READING SHADER " << pathToShader << endl;
        exit(EXIT_FAILURE);
    }
    ostringstream fileContent;
    fileContent << fileReader.rdbuf();
    fileReader.close();

    const string temporaryString = fileContent.str();
    const char* shaderCode = temporaryString.data();

    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);

    int successful;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &successful);
    if(!successful){
        char errorLog[512];
        glGetShaderInfoLog(shaderID, 512, NULL, errorLog);
        cerr << "ERROR COMPILING SHADER " << pathToShader << ": " << errorLog << endl;
        exit(EXIT_FAILURE);
    }
        
    return shaderID;
}

void Renderer::initializeOpenGLShadingSubroutines() {
    for (ShadingMethod& method : ShadingMethod::methods) {
        method.vertexSubroutine.index = glGetSubroutineIndex(
                openGLProgram, 
                GL_VERTEX_SHADER, 
                method.vertexSubroutine.name
        );
        method.fragmentSubroutine.index = glGetSubroutineIndex(
                openGLProgram, 
                GL_FRAGMENT_SHADER, 
                method.fragmentSubroutine.name
        );
    }
}

mat4 Renderer::openGLProjectionMatrix() {
    float aspectRatio = (float) Window::width / (float) Window::height;
    mat4 projectionMatrix = perspective(
            radians(Renderer::verticalFieldOfView),
            aspectRatio, 
            Renderer::nearPlane, 
            Renderer::farPlane
    );

    return projectionMatrix;
}

mat4 Renderer::openGLViewMatrix() {
    mat4 viewMatrix = lookAt(
            Camera::position, 
            Camera::position+Camera::direction, 
            Camera::up
    );
    return viewMatrix;
}

void Renderer::setShadingMethod(ShadingMethod method) {
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &(method.vertexSubroutine.index));
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &(method.fragmentSubroutine.index));
}
