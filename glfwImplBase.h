//
// Created by wangmuy on 09/05/2017.
//

#ifndef GLITTER_GLFWIMPLBASE_H
#define GLITTER_GLFWIMPLBASE_H

#include <GLFW/glfw3.h>

typedef struct ImplCallback {
    void* (*getUserData)();
    void (*setWindow)(GLFWwindow* window, void* userData);
    int (*onInit)(void* userData);
    int (*onRender)(void* userData);
    void (*onKeyCallback)(GLFWwindow* window, int key, int scancode, int action, int mode, void* userData);
} ImplCallback;

#endif //GLITTER_GLFWIMPLBASE_H
