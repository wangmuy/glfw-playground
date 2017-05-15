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

#define RES_DIR "../../Glitter/Sources/impl/res/"

struct MyData {
    GLFWwindow* mWindow;
    GLint screenWidth, screenHeight;
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
    glm::vec3 mCameraPos, mCameraFront, mCameraUp;
    GLfloat fov;
    GLfloat deltaTime, lastFrameTime;
    GLfloat lastX, lastY;
    GLfloat pitch, yaw;
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
static void onMouseCallback(GLFWwindow* window, double xpos, double ypos);
static void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
ImplCallback g_implCamera = {getUserData, setWindow, onInit, onRender, onKeyCallback};

static void setWindow(GLFWwindow* window, void* userData) {
    MyData& d = *(MyData*)userData;
    d.mWindow = window;
}

static void* getUserData() {
    return &sData;
}

static bool keys[1024];

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

    glGenBuffers(1, &d.VBO);
    glGenBuffers(1, &d.EBO);
    glGenVertexArrays(1, &d.VAO);

    glBindVertexArray(d.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
    glBufferData(GL_ARRAY_BUFFER, d.mVertSize, d.mVertices, GL_STATIC_DRAW);
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
    glfwGetFramebufferSize(d.mWindow, &d.screenWidth, &d.screenHeight);
    glm::mat4 identity;
    d.mModel = glm::rotate(identity, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
    d.mModelLoc = glGetUniformLocation(d.mShaderProgram, "model");
    d.mCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    d.yaw = -90.0f; d.pitch = 0.0f;
    d.mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    d.mCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//    d.mView =  glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    d.mViewLoc = glGetUniformLocation(d.mShaderProgram, "view");
//    d.mProjection = glm::perspective(glm::radians(45.0f), screenWidth/(GLfloat)screenHeight, 0.1f, 100.0f);
    d.fov = 30.0f;
    d.mProjectionLoc = glGetUniformLocation(d.mShaderProgram, "projection");

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
    GLfloat currentFrameTime = glfwGetTime();
    d.deltaTime = currentFrameTime - d.lastFrameTime;
    d.lastFrameTime = currentFrameTime;
    GLfloat cameraSpeed = 5.0f * d.deltaTime;
    if(keys[GLFW_KEY_W])
        d.mCameraPos += cameraSpeed * d.mCameraFront;
    if(keys[GLFW_KEY_S])
        d.mCameraPos -= cameraSpeed * d.mCameraFront;
    if(keys[GLFW_KEY_A])
        d.mCameraPos += glm::normalize(glm::cross(d.mCameraFront, d.mCameraUp)) * cameraSpeed;
    if(keys[GLFW_KEY_D])
        d.mCameraPos -= glm::normalize(glm::cross(d.mCameraFront, d.mCameraUp)) * cameraSpeed;
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
    glUseProgram(d.mShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, d.mTexture1);
    glUniform1i(glGetUniformLocation(d.mShaderProgram, "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, d.mTexture2);
    glUniform1i(glGetUniformLocation(d.mShaderProgram, "ourTexture2"), 1);

//    GLfloat radius = 10.0f;
//    GLfloat camX = sin(glfwGetTime()) * radius;
//    GLfloat camZ = cos(glfwGetTime()) * radius;
//    d.mView = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

//    d.mView = glm::lookAt(d.mCameraPos, d.mCameraPos+d.mCameraFront, d.mCameraUp);
    d.mView = myLookAt(d.mCameraPos, d.mCameraPos+d.mCameraFront, d.mCameraUp);

    glUniformMatrix4fv(d.mViewLoc, 1, GL_FALSE, glm::value_ptr(d.mView));
    d.mProjection = glm::perspective(d.fov, d.screenWidth/(GLfloat)d.screenHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(d.mProjectionLoc, 1, GL_FALSE, glm::value_ptr(d.mProjection));

    glBindVertexArray(d.VAO);
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

    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    d.yaw -= xoffset;
    d.pitch -= yoffset;
//    fprintf(stderr, "last=(%.2f,%.2f), offset=(%.2f,%.2f), pitch,yaw=(%.2f,%.2f)\n",
//            d.lastX, d.lastY, xoffset, yoffset, d.pitch, d.yaw);
    if(d.pitch > 89.0f)
        d.pitch = 89.0f;
    if(d.pitch < -89.0f)
        d.pitch = -89.0f;

    glm::vec3 norm;
    glm::vec3 front;
    front.x = (GLfloat)(cos(glm::radians(d.pitch)) * cos(glm::radians(d.yaw)));
    front.y = (GLfloat)sin(glm::radians(d.pitch));
    front.z = (GLfloat)(cos(glm::radians(d.pitch)) * sin(glm::radians(d.yaw)));
    norm = glm::normalize(front);

//    std::cerr<< "camFront=" << glm::to_string(d.mCameraFront)
//             << ", front=" << glm::to_string(front)
//             << ", norm=" << glm::to_string(norm) <<std::endl;
    d.mCameraFront = norm;
}

static void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    GLfloat sensitivity = 0.05f;
    MyData& d = sData;
    if(d.fov >= 1.0f && d.fov < 45.0f)
        d.fov += yoffset*sensitivity;
    if(d.fov <= 1.0f)
        d.fov = 1.0f;
    if(d.fov >= 45.0f)
        d.fov = 45.0f;
}


/***** Class *****/
// Constructor with vectors
GLFWImpl::Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

// Constructor with scalar values
GLFWImpl::Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
                         GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 GLFWImpl::Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

// Processes input received from any keyboard-like input system.
// Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void GLFWImpl::Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->Front * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Front * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void GLFWImpl::Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw   += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void GLFWImpl::Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void GLFWImpl::Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = (GLfloat)(cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch)));
    front.y = (GLfloat)sin(glm::radians(this->Pitch));
    front.z = (GLfloat)(sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch)));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    // Normalize the vectors, because their length gets closer to 0
    // the more you look up or down which results in slower movement.
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}
