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
#include "glfwImplCamera.h"
#include "glfwImplShader.h"

#define RES_DIR "../../Glitter/Sources/impl/res/"

struct UniformLocBase {
    GLint mModelLoc;
    GLint mViewLoc;
    GLint mProjectionLoc;
};

struct ObjectUniformLoc : public UniformLocBase {
    GLint mObjectColorLoc;
    GLint mLightColorLoc;
    GLint mLightPosLoc;
    GLint mViewPosLoc;
};

struct MyData {
    GLFWwindow* mWindow;
    GLint screenWidth, screenHeight;
    GLfloat* mVertices;
    GLuint mVertSize;
    GLuint VBO;
    GLuint VAO;
    GLuint lightVAO;
    GLuint* mIndexes;
    GLuint mIndexSize;
    GLFWImpl::Shader mObjectShader;
    GLFWImpl::Shader mLampShader;
    GLuint mTexture1;
    GLuint mTexture2;
    glm::mat4 mModel;
    GLfloat deltaTime, lastFrameTime;
    GLfloat lastX, lastY;
    GLFWImpl::Camera mCamera;
    glm::mat4 mView;
    glm::mat4 mProjection;
    UniformLocBase mLampUniforms;
    ObjectUniformLoc mObjectUniforms;
    glm::vec3 lightPos;
    glm::mat4 lightModel;
    MyData() : mObjectShader(NULL, NULL), mLampShader(NULL, NULL) {}
};
static MyData sData;

static void* getUserData();
static void setWindow(GLFWwindow* window, void* userData);
static int onInit(void* userData);
static int onRender(void* userData);
static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode, void* userData);
static void onMouseCallback(GLFWwindow* window, double xpos, double ypos);
static void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
ImplCallback g_implBasicLighting = {getUserData, setWindow, onInit, onRender, onKeyCallback};

static void setWindow(GLFWwindow* window, void* userData) {
    MyData& d = *(MyData*)userData;
    d.mWindow = window;
}

static void* getUserData() {
    return &sData;
}

static bool keys[1024];

static GLfloat verticesCube[] = {
        // vertex coord      // normal
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

static const char* objectVertShaderSource =""
        "#version 330 core\n"
        "layout (location=0) in vec3 position;\n"
        "layout (location=1) in vec3 normal;\n"
        "out vec3 FragPos;\n"
        "out vec3 Normal;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main() {\n"
        "  gl_Position = projection * view * model * vec4(position, 1.0);\n"
        "  FragPos = vec3(model * vec4(position, 1.0f));\n"
        "  Normal = mat3(transpose(inverse(model))) * normal; // TODO: do trans/inv in CPU side\n"
        "}\n"
"\n";

static const char* objectFragShaderSource =""
        "#version 330 core\n"
        "in vec3 FragPos;\n"
        "in vec3 Normal;\n"
        "out vec4 color;\n"
        "uniform vec3 objectColor;\n"
        "uniform vec3 lightColor;\n"
        "uniform vec3 lightPos;\n"
        "uniform vec3 viewPos;\n"
        "void main() {\n"
        "  float ambientStrength = 0.1f;\n"
        "  vec3 ambient = ambientStrength * lightColor;\n"
        "  vec3 norm = normalize(Normal);\n"
        "  vec3 lightDir = normalize(lightPos - FragPos);\n"
        "  float diff = max(dot(norm, lightDir), 0.0);\n"
        "  vec3 diffuse = diff * lightColor;\n"
        "  float specularStrength = 0.5f;\n"
        "  vec3 viewDir = normalize(viewPos - FragPos);\n"
        "  vec3 reflectDir = reflect(-lightDir, norm);\n"
        "  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "  vec3 specular = specularStrength * spec * lightColor;\n"
        "  vec3 result = (ambient + diffuse + specular) * objectColor;\n"
        "  color = vec4(result, 1.0f);\n"
        "}\n"
"\n";

static const char* lampVertShaderSource = ""
        "#version 330 core\n"
        "layout (location=0) in vec3 position;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main() {\n"
        "  gl_Position = projection * view * model * vec4(position, 1.0);\n"
        "}\n"
"\n";

static const char* lampFragShaderSource = ""
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "  color = vec4(1.0f);\n"
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

    // Object
    glGenBuffers(1, &d.VBO);
    glGenVertexArrays(1, &d.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
    glBufferData(GL_ARRAY_BUFFER, d.mVertSize, d.mVertices, GL_STATIC_DRAW);
    glBindVertexArray(d.VAO);
    // position attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // tex attrib
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Light
    glGenVertexArrays(1, &d.lightVAO);
    glBindVertexArray(d.lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, d.VBO); // same VBO already contains the correct data.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // shader
    d.mObjectShader.init(objectVertShaderSource, objectFragShaderSource);
    if(!d.mObjectShader.ready)
        return ret;
    d.mLampShader.init(lampVertShaderSource, lampFragShaderSource);
    if(!d.mLampShader.ready)
        return ret;
    d.lightPos = glm::vec3(1.2f, 1.0f, 1.0f);
    d.lightModel = glm::translate(glm::mat4(), d.lightPos);
    d.lightModel = glm::scale(d.lightModel, glm::vec3(0.2f));

    // trans
    glfwGetFramebufferSize(d.mWindow, &d.screenWidth, &d.screenHeight);
    glm::mat4 identity;
    d.mModel = glm::rotate(identity, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
    d.mObjectUniforms.mModelLoc = glGetUniformLocation(d.mObjectShader.Program, "model");
    d.mCamera = GLFWImpl::Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    d.mObjectUniforms.mViewLoc = glGetUniformLocation(d.mObjectShader.Program, "view");
    d.mObjectUniforms.mProjectionLoc = glGetUniformLocation(d.mObjectShader.Program, "projection");

    // uniforms
    d.mObjectUniforms.mObjectColorLoc = glGetUniformLocation(d.mObjectShader.Program, "objectColor");
    d.mObjectUniforms.mLightColorLoc = glGetUniformLocation(d.mObjectShader.Program, "lightColor");
    d.mObjectUniforms.mLightPosLoc = glGetUniformLocation(d.mObjectShader.Program, "lightPos");
    d.mObjectUniforms.mViewPosLoc = glGetUniformLocation(d.mObjectShader.Program, "viewPos");

    d.mLampUniforms.mModelLoc = glGetUniformLocation(d.mLampShader.Program, "model");
    d.mLampUniforms.mViewLoc = glGetUniformLocation(d.mLampShader.Program, "view");
    d.mLampUniforms.mProjectionLoc = glGetUniformLocation(d.mLampShader.Program, "projection");
//    fprintf(stderr,
//            "object uniforms loc=(%d %d %d %d %d)\n"
//            "light  uniforms loc=(%d %d %d)",
//            d.mObjectUniforms.mModelLoc, d.mObjectUniforms.mViewLoc, d.mObjectUniforms.mProjectionLoc,
//            d.mObjectUniforms.mObjectColorLoc, d.mObjectUniforms.mLightColorLoc,
//            d.mLampUniforms.mModelLoc, d.mLampUniforms.mViewLoc, d.mLampUniforms.mProjectionLoc);

    // mouse
    glfwSetInputMode(d.mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(d.mWindow, onMouseCallback);
    d.lastX = d.screenWidth/2; d.lastY = d.screenHeight/2;

    // scroll
    glfwSetScrollCallback(d.mWindow, onScrollCallback);

    // misc
    glEnable(GL_DEPTH_TEST);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);// GL_LINE

    return 1;
}

static void do_movement(void* userData) {
    MyData& d = *(MyData*)userData;
    GLfloat currentFrameTime = (GLfloat)glfwGetTime();
    d.deltaTime = currentFrameTime - d.lastFrameTime;
    d.lastFrameTime = currentFrameTime;
    if(keys[GLFW_KEY_W])
        d.mCamera.ProcessKeyboard(GLFWImpl::Camera_Movement::FORWARD, d.deltaTime);
    if(keys[GLFW_KEY_S])
        d.mCamera.ProcessKeyboard(GLFWImpl::Camera_Movement::BACKWARD, d.deltaTime);
    if(keys[GLFW_KEY_A])
        d.mCamera.ProcessKeyboard(GLFWImpl::Camera_Movement::LEFT, d.deltaTime);
    if(keys[GLFW_KEY_D])
        d.mCamera.ProcessKeyboard(GLFWImpl::Camera_Movement::RIGHT, d.deltaTime);
}

static glm::mat4 myLookAt(const glm::vec3& camPos, const glm::vec3& camTarget, const glm::vec3& up) {
    glm::mat4 RUD;
    glm::vec3 cameraDirection = glm::normalize(camPos - camTarget);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    RUD[0] = glm::vec4(cameraRight, 0.0f);
    RUD[1] = glm::vec4(cameraUp, 0.0f);
    RUD[2] = glm::vec4(cameraDirection, 0.0f);
    RUD[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    RUD = glm::transpose(RUD); // column based
    glm::mat4 minusP = glm::translate(glm::mat4(), -camPos);
    return RUD*minusP;
}

static int onRender(void* userData) {
    do_movement(userData);

    MyData& d = *(MyData*)userData;
//    d.mView = glm::lookAt(d.mCamera.Position, d.mCamera.Position+d.mCamera.Front, d.mCamera.Up);
    d.mView = myLookAt(d.mCamera.Position, d.mCamera.Position+d.mCamera.Front, d.mCamera.Up);
    d.mProjection = glm::perspective(d.mCamera.Zoom, d.screenWidth/(GLfloat)d.screenHeight, 0.1f, 100.0f);

    d.mObjectShader.Use();
    glUniformMatrix4fv(d.mObjectUniforms.mViewLoc, 1, GL_FALSE, glm::value_ptr(d.mView));
    glUniformMatrix4fv(d.mObjectUniforms.mProjectionLoc, 1, GL_FALSE, glm::value_ptr(d.mProjection));
    glUniformMatrix4fv(d.mObjectUniforms.mModelLoc, 1, GL_FALSE, glm::value_ptr(d.mModel));
    glUniform3f(d.mObjectUniforms.mObjectColorLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(d.mObjectUniforms.mLightColorLoc, 1.0f, 1.0f, 1.0f);
    d.lightPos = glm::vec3(1.2f*cos(glfwGetTime()), 1.0f, 1.0f*sin(glfwGetTime()));
    glUniform3f(d.mObjectUniforms.mLightPosLoc, d.lightPos.x, d.lightPos.y, d.lightPos.z);
    glUniform3f(d.mObjectUniforms.mViewPosLoc, d.mCamera.Position.x, d.mCamera.Position.y, d.mCamera.Position.z);
    glBindVertexArray(d.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


    d.mLampShader.Use();
    d.lightModel = glm::translate(glm::mat4(), d.lightPos);
    d.lightModel = glm::scale(d.lightModel, glm::vec3(0.2f));
    glUniformMatrix4fv(d.mLampUniforms.mModelLoc, 1, GL_FALSE, glm::value_ptr(d.lightModel));
    glUniformMatrix4fv(d.mLampUniforms.mViewLoc, 1, GL_FALSE, glm::value_ptr(d.mView));
    glUniformMatrix4fv(d.mLampUniforms.mProjectionLoc, 1, GL_FALSE, glm::value_ptr(d.mProjection));
    glBindVertexArray(d.lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    return 1;
}

static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode, void* userData) {
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

static bool firstMove = true;
static void onMouseCallback(GLFWwindow* window, double xpos, double ypos) {
    MyData& d = sData;
    if(firstMove) {
        d.lastX = (GLfloat)xpos;
        d.lastY = (GLfloat)ypos;
        firstMove = false;
    }

    GLfloat xoffset = (GLfloat)xpos - d.lastX;
    GLfloat yoffset = d.lastY - (GLfloat)ypos; // reverse since y-coord range from bottom to top
    d.lastX = (GLfloat)xpos;
    d.lastY = (GLfloat)ypos;

    d.mCamera.ProcessMouseMovement(xoffset, yoffset);
}

static void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    MyData& d = sData;
    d.mCamera.ProcessMouseScroll(yoffset);
}
