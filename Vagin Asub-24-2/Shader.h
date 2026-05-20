#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GL/glew.h"

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile(vertexPath);
        std::ifstream fShaderFile(fragmentPath);
        std::stringstream vStream, fStream;
        vStream << vShaderFile.rdbuf();
        fStream << fShaderFile.rdbuf();
        vertexCode = vStream.str();
        fragmentCode = fStream.str();
        const char* vCode = vertexCode.c_str();
        const char* fCode = fragmentCode.c_str();

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vCode, NULL);
        glCompileShader(vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fCode, NULL);
        glCompileShader(fragment);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void setMat4(const std::string name, const glm::mat4 mat) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void use() {
        glUseProgram(ID);
    }
};

#endif