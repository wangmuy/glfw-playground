//
// Created by wangmuy on 09/05/2017.
//

#include <glad/glad.h>
#include <cstdio>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glfwImplBase.h"
#include "glfwImplShader.h"

#define RES_DIR "../../Glitter/Sources/impl/res/"

struct MyData {
    GLFWwindow* mWindow;
    GLfloat* mVertices;
    GLuint mVertSize;
    GLuint VBO;
    GLuint VAO;
    GLuint* mIndexes;
    GLuint mIndexSize;
    GLuint EBO;
    GLuint mShaderProgram;
    GLint mVertexColorLocation;
};
static MyData sData;

static void* getUserData();
static void setWindow(GLFWwindow* window, void* userData);
static int onInit(void* userData);
static int onRender(void* userData);
static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode, void* userData);
ImplCallback g_implShaders = {getUserData, setWindow, onInit, onRender, onKeyCallback};

static void setWindow(GLFWwindow* window, void* userData) {
    MyData& d = *(MyData*)userData;
    d.mWindow = window;
}

static void* getUserData() {
    return &sData;
}

static GLfloat vertices[] = {
        // positions        // colors
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, // top left
};
static GLuint indices[] = {
        0, 1, 3, // fst tri
        1, 2, 3  // snd tri
};

static const char* vertShaderSource =""
        "#version 330 core\n"
        "layout (location=0) in vec3 position;\n"
        "layout (location=1) in vec3 color;\n"
        "out vec4 vertexColor;\n"
        "void main() {\n"
        "  gl_Position = vec4(position, 1.0);\n"
        "vertexColor = vec4(color, 1.0); //vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
        "}\n"
"\n";

static const char* fragShaderSource =""
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main() {\n"
        "  color = vertexColor; // ourColor; // vertexColor; //vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n"
"\n";

static GLint checkShaderCompileStatus(GLuint shader) {
    GLint success=0;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }
    return success;
}

static int onInit(void* userData) {
    int ret = 0;
    MyData& d = *(MyData*)userData;
    d.mVertices = vertices;
    d.mVertSize = sizeof(vertices);
    d.mIndexes = indices;
    d.mIndexSize = sizeof(indices);

    glGenBuffers(1, &d.VBO);
    glGenBuffers(1, &d.EBO);
    glGenVertexArrays(1, &d.VAO);

    glBindVertexArray(d.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
    glBufferData(GL_ARRAY_BUFFER, d.mVertSize, d.mVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, d.mIndexSize, d.mIndexes, GL_STATIC_DRAW);
    // position attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // color attrib
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertShaderSource, NULL);
    glCompileShader(vertShader);
    ret=checkShaderCompileStatus(vertShader);
    if(!ret)
        return ret;

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);
    ret=checkShaderCompileStatus(fragShader);
    if(!ret)
        return ret;

    d.mShaderProgram = glCreateProgram();
    glAttachShader(d.mShaderProgram, vertShader);
    glAttachShader(d.mShaderProgram, fragShader);
    glLinkProgram(d.mShaderProgram);
    glGetProgramiv(d.mShaderProgram, GL_LINK_STATUS, &ret);
    if(!ret) {
        GLchar infoLog[512];
        glGetProgramInfoLog(d.mShaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINK_FAILED\n%s", infoLog);
        return ret;
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

//    d.mShaderProgram = GLFWImpl::Shader(RES_DIR "shader_v.shader",
//                                        RES_DIR "shader_f.shader").Program;

    d.mVertexColorLocation = glGetUniformLocation(d.mShaderProgram, "ourColor");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);// GL_LINE

    return 1;
}

static int onRender(void* userData) {
    MyData& d = *(MyData*)userData;
    glUseProgram(d.mShaderProgram);
//    GLfloat timeValue = glfwGetTime();
//    GLfloat greenValue = (sin(timeValue)/2) + 0.5;
//    glUniform4f(d.mVertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    glBindVertexArray(d.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return 1;
}

static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode, void* userData) {
    ;
}


// Shader class

GLFWImpl::Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void GLFWImpl::Shader::Use()
{
    glUseProgram(this->Program);
}
