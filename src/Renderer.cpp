#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using std::ifstream;
using std::ostringstream;
using std::cerr;
using std::endl;
using std::exit;

#define VERTEX_SHADER_LOCATION "../shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_LOCATION "../shaders/fragment_shader.glsl"

Renderer::Renderer() {

    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "HELLO WORLD", NULL, NULL);
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f
    };  

    unsigned int vertexBufferID, vertexArrayID;

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    this->vertexArrayID = vertexArrayID;
}

Renderer::~Renderer() {
    glfwTerminate();
}

void Renderer::render() {
    glClearColor(0.43f, 0.89f, 0.78f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgramID);
    glBindVertexArray(vertexArrayID);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
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
