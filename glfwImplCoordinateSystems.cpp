//
// Created by wangmuy on 09/05/2017.
//

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
// std::cerr<< glm::to_string(glm::mat4()) <<std::endl;

#include <cstdio>
#include <stb_image.h>
#include <iostream>
#include "glfwImplBase.h"

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
    GLuint mTexture1;
    GLuint mTexture2;
    glm::mat4 mModel;
    glm::mat4 mView;
    glm::mat4 mProjection;
    GLint mModelLoc;
    GLint mViewLoc;
    GLint mProjectionLoc;
};
static MyData sData;

static void* getUserData();
static void setWindow(GLFWwindow* window, void* userData);
static int onInit(void* userData);
static int onRender(void* userData);
static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode, void* userData);
ImplCallback g_implCoordinateSystems = {getUserData, setWindow, onInit, onRender, onKeyCallback};

static void setWindow(GLFWwindow* window, void* userData) {
    MyData& d = *(MyData*)userData;
    d.mWindow = window;
}

static void* getUserData() {
    return &sData;
}

static GLfloat vertices[] = {
        // positions        // tex coords
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left
};
static GLuint indices[] = {
        0, 1, 3, // fst tri
        1, 2, 3  // snd tri
};

static GLfloat verticesCube[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
static glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};

static const char* vertShaderSource =""
        "#version 330 core\n"
        "layout (location=0) in vec3 position;\n"
        "layout (location=1) in vec2 texCoord;\n"
        "out vec2 TexCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main() {\n"
        "  gl_Position = projection * view * model * vec4(position, 1.0);\n"
        "TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);\n"
        "}\n"
"\n";

static const char* fragShaderSource =""
        "#version 330 core\n"
        "in vec2 TexCoord;\n"
        "out vec4 color;\n"
        "uniform sampler2D ourTexture1;\n"
        "uniform sampler2D ourTexture2;\n"
        "void main() {\n"
        "  color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);\n"
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
    d.mVertices = verticesCube;
    d.mVertSize = sizeof(verticesCube);
//    d.mVertices = vertices;
//    d.mVertSize = sizeof(vertices);
//    d.mIndexes = indices;
//    d.mIndexSize = sizeof(indices);

    glGenBuffers(1, &d.VBO);
    glGenBuffers(1, &d.EBO);
    glGenVertexArrays(1, &d.VAO);

    glBindVertexArray(d.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
    glBufferData(GL_ARRAY_BUFFER, d.mVertSize, d.mVertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, d.mIndexSize, d.mIndexes, GL_STATIC_DRAW);
    // position attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // tex attrib
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // shader
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

    // texture
    int texWidth, texHeight, texChannels;
    const char* imageFile = NULL;
    unsigned char* image = NULL;
    // tex1
    glGenTextures(1, &d.mTexture1);
    glBindTexture(GL_TEXTURE_2D, d.mTexture1);
    imageFile = RES_DIR "container.jpg";
    image = stbi_load(imageFile, &texWidth, &texHeight, &texChannels, 0);
    if (!image) {
        fprintf(stderr, "%s %s\n", "Failed to Load Texture:", imageFile);
        return 0; // failed
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    image = NULL;
    // tex2
    glGenTextures(1, &d.mTexture2);
    glBindTexture(GL_TEXTURE_2D, d.mTexture2);
    imageFile = RES_DIR "awesomeface.png";
    image = stbi_load(imageFile, &texWidth, &texHeight, &texChannels, 0);
    if (!image) {
        fprintf(stderr, "%s %s\n", "Failed to Load Texture:", imageFile);
        return 0; // failed
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    image = NULL;

    // trans
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(d.mWindow, &screenWidth, &screenHeight);
    glm::mat4 identity;
    d.mModel = glm::rotate(identity, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
    d.mModelLoc = glGetUniformLocation(d.mShaderProgram, "model");
    d.mView =  glm::translate(identity, glm::vec3(0.0f, 0.0f, -3.0f));
    d.mViewLoc = glGetUniformLocation(d.mShaderProgram, "view");
    d.mProjection = glm::perspective(glm::radians(45.0f), screenWidth/(GLfloat)screenHeight, 0.1f, 100.0f);
    d.mProjectionLoc = glGetUniformLocation(d.mShaderProgram, "projection");

    // misc
    glEnable(GL_DEPTH_TEST);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);// GL_LINE

    return 1;
}

static int onRender(void* userData) {
    MyData& d = *(MyData*)userData;
    glUseProgram(d.mShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, d.mTexture1);
    glUniform1i(glGetUniformLocation(d.mShaderProgram, "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, d.mTexture2);
    glUniform1i(glGetUniformLocation(d.mShaderProgram, "ourTexture2"), 1);


    glUniformMatrix4fv(d.mViewLoc, 1, GL_FALSE, glm::value_ptr(d.mView));
    glUniformMatrix4fv(d.mProjectionLoc, 1, GL_FALSE, glm::value_ptr(d.mProjection));

    glBindVertexArray(d.VAO);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    for(GLuint i=0; i < sizeof(cubePositions)/sizeof(cubePositions[0]); i++) {
        d.mModel = glm::translate(glm::mat4(), cubePositions[i]);
        GLfloat angle = 20.0f * i;
        int factor = (i%3==0? i+1 : 0); // every 3rd
        d.mModel = glm::rotate(d.mModel, glm::radians( angle+(GLfloat)glfwGetTime()*30.0f*factor ), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(d.mModelLoc, 1, GL_FALSE, glm::value_ptr(d.mModel));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    return 1;
}

static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode, void* userData) {
    ;
}
