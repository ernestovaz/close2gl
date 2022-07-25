#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Settings.h"
#include "../application/Window.h"

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

int Renderer::currentShadingMethod = 1;

Renderer::Renderer() {
    if(!gladLoadGLLoader((GLADloadproc) Window::getProcessAddress())) {
        cerr << "ERROR INITIALIZING GLAD" << endl;
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_PROGRAM_POINT_SIZE);
    unsigned int vertexShaderID, fragmentShaderID;
    vertexShaderID = loadShader(VERTEX_SHADER_LOCATION, GL_VERTEX_SHADER);
    fragmentShaderID = loadShader(FRAGMENT_SHADER_LOCATION, GL_FRAGMENT_SHADER);

    shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    modelUniformID = glGetUniformLocation(shaderProgramID, "model");
    viewUniformID = glGetUniformLocation(shaderProgramID, "view");
    projectionUniformID = glGetUniformLocation(shaderProgramID, "projection");
    colorUniformID = glGetUniformLocation(shaderProgramID, "color");

    initializeModelMatrix();
    updateProjectionMatrix();
    updateViewMatrix();

    initializeShadingSubroutines();
}

void Renderer::setModel(Model model) {
    unsigned int vertexArrayID, elementArrayBufferID,
                 positionBufferID, normalBufferID;

    this->model = model;

    // set up VAO
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // set up EBO
    glGenBuffers(1, &elementArrayBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferID);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, 
            this->model.indices.size() * sizeof(unsigned int), 
            this->model.indices.data(), 
            GL_STATIC_DRAW
    );

    // set up VBOs
    // positions
    glGenBuffers(1, &positionBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferID);
    glBufferData(
            GL_ARRAY_BUFFER, 
            this->model.positions.size() * sizeof(vec3), 
            this->model.positions.data(), 
            GL_STATIC_DRAW
    );
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glEnableVertexAttribArray(0);

    // normals
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(
            GL_ARRAY_BUFFER, 
            this->model.normals.size() * sizeof(vec3), 
            this->model.normals.data(), 
            GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

    this->vertexArrayID = vertexArrayID;

}

void Renderer::render() {
    glClearColor(0.10f, 0.09f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vertexArrayID);
    setShadingMethod(ShadingMethod::methods.at(currentShadingMethod)); 

    checkAndUpdateViewMatrix();
    checkAndUpdateProjectionMatrix();
    glUniform3fv(colorUniformID, 1, value_ptr(Settings::renderingColor));


    switch(Settings::renderingPrimitive){
        case RenderingPrimitive::TRIANGLES:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case RenderingPrimitive::LINES:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case RenderingPrimitive::POINTS:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;

    }

    if(Settings::cullingEnabled) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);

    if(Settings::reverseFaceOrientation) glFrontFace(GL_CW);
    else glFrontFace(GL_CCW);

    glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
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

void Renderer::initializeShadingSubroutines() {
    for (ShadingMethod& method : ShadingMethod::methods) {
        method.vertexSubroutine.index = glGetSubroutineIndex(
                shaderProgramID, 
                GL_VERTEX_SHADER, 
                method.vertexSubroutine.name
        );
        method.fragmentSubroutine.index = glGetSubroutineIndex(
                shaderProgramID, 
                GL_FRAGMENT_SHADER, 
                method.fragmentSubroutine.name
        );
    }
}

void Renderer::initializeModelMatrix() {
    glUniformMatrix4fv(modelUniformID,1,GL_FALSE, value_ptr(mat4(1.0f)));
}

void Renderer::updateProjectionMatrix() {
    float aspectRatio = (float) Window::width / (float) Window::height;
    mat4 projectionMatrix = perspective(
            radians(Settings::fieldOfView), 
            aspectRatio, 
            Settings::nearPlane, 
            Settings::farPlane
    );
    glUniformMatrix4fv(projectionUniformID,1,GL_FALSE, value_ptr(projectionMatrix));
}

void Renderer::updateViewMatrix() {
    mat4 viewMatrix = lookAt(
            Camera::position, 
            Camera::position+Camera::direction, 
            Camera::up
    );
    glUniformMatrix4fv(viewUniformID,1,GL_FALSE, value_ptr(viewMatrix));
}

void Renderer::checkAndUpdateProjectionMatrix() {
    if (Window::hasSizeChanged) {
        glViewport(0, 0, Window::width, Window::height);
        Window::hasSizeChanged = false;
    }
    updateProjectionMatrix();
}

void Renderer::checkAndUpdateViewMatrix() {
    if (Camera::hasChanged) {
        updateViewMatrix();
        Camera::hasChanged = false;
    }
}

void Renderer::setShadingMethod(ShadingMethod method) {
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &(method.vertexSubroutine.index));
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &(method.fragmentSubroutine.index));
}
