#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::ifstream;
using std::ostringstream;
using std::cerr;
using std::endl;
using std::exit;

using glm::perspective;
using glm::value_ptr;
using glm::rotate;
using glm::vec3;
using glm::radians;

#define VERTEX_SHADER_LOCATION "../shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_LOCATION "../shaders/fragment_shader.glsl"

Renderer::Renderer(const char* windowProcessAddress, mat4 viewMatrix) {
    if(!gladLoadGLLoader((GLADloadproc)windowProcessAddress)) {
        exit(EXIT_FAILURE);
    }

    unsigned int vertexShaderID, fragmentShaderID;
    vertexShaderID = loadShader(VERTEX_SHADER_LOCATION, GL_VERTEX_SHADER);
    fragmentShaderID = loadShader(FRAGMENT_SHADER_LOCATION, GL_FRAGMENT_SHADER);
    this->shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    // placeholder triangle model
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f, 
         0.0f,  0.5f, 0.0f, 
    };  

    unsigned int vertexBufferID, vertexArrayID;

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    this->fieldOfView = DEFAULT_FIELD_OF_VIEW;
    this->aspectRatio = DEFAULT_ASPECT_RATIO;
    this->nearPlane = DEFAULT_NEAR_PLANE;
    this->farPlane = DEFAULT_FAR_PLANE;

    mat4 projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane);

    this->vertexArrayID = vertexArrayID;

    this->modelMatrix = mat4(1.0f);
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projection;

    this->modelUniformID = glGetUniformLocation(shaderProgramID, "model");
    this->viewUniformID = glGetUniformLocation(shaderProgramID, "view");
    this->projectionUniformID = glGetUniformLocation(shaderProgramID, "projection");

    glUniformMatrix4fv(modelUniformID,1,GL_FALSE, value_ptr(modelMatrix));
    glUniformMatrix4fv(viewUniformID,1,GL_FALSE, value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionUniformID,1,GL_FALSE, value_ptr(projectionMatrix));

}

void Renderer::render() {
    glClearColor(0.43f, 0.89f, 0.78f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgramID);
    glBindVertexArray(vertexArrayID);

    count++;
    this->viewMatrix = rotate(viewMatrix,radians((float)count),vec3(0.0, 1.0, 0.0));

    glUniformMatrix4fv(modelUniformID,1,GL_FALSE, value_ptr(modelMatrix));
    glUniformMatrix4fv(viewUniformID,1,GL_FALSE, value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionUniformID,1,GL_FALSE, value_ptr(projectionMatrix));

    glDrawArrays(GL_TRIANGLES, 0, 3);
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

    return programID;
}
