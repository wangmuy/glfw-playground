//
// Created by wangmuy on 2017/5/13.
//

#ifndef GLITTER_GLFWIMPLSHADER_H_H
#define GLITTER_GLFWIMPLSHADER_H_H

#include <GLFW/glfw3.h>

namespace GLFWImpl
{
    class Shader {
    public:
        GLuint Program;

        Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

        void Use();
    };
}

#endif //GLITTER_GLFWIMPLSHADER_H_H
