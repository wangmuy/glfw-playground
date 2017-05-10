//
// Created by wangmuy on 09/05/2017.
//

#include <OpenGL/OpenGL.h>
#include <glad/glad.h>
#include <cstdio>
#include "glfwImplBase.h"

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
};
MyData sData;

static void* getUserData();
static void setWindow(GLFWwindow* window, void* userData);
static int onInit(void* userData);
static int onRender(void* userData);
static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
ImplBase g_implHelloTriangle = {getUserData, setWindow, onInit, onRender, onKeyCallback};

static void setWindow(GLFWwindow* window, void* userData) {
    MyData& d = *(MyData*)userData;
    d.mWindow = window;
}

static void* getUserData() {
    return &sData;
}

static GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f  // top left
};

static GLuint indices[] = {
        0, 1, 3, // fst tri
        1, 2, 3  // snd tri
};

static const char* vertShaderSource =""
        "#version 330 core\n"
        "layout (location=0) in vec3 position;\n"
        "void main() {\n"
        "  gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\n"
"\n";

static const char* fragShaderSource =""
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "  color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n"
"\n";

GLint checkShaderCompileStatus(GLuint shader) {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
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

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);// GL_LINE

    return 1;
}

static int onRender(void* userData) {
    MyData& d = *(MyData*)userData;
    glUseProgram(d.mShaderProgram);
    glBindVertexArray(d.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return 1;
}

static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    ;
}
