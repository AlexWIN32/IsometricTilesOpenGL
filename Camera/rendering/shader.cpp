//based on https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h

#include "shader.h"
#include <consoleLog.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

bool Shader::LoadFromFiles(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexShaderFilePath);
        fShaderFile.open(fragmentShaderFilePath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    } catch (std::ifstream::failure& e) {
        LOG_ERROR("shader file reading error %s", e.what());
        return false;
    }

    return LoadFromCode(vertexCode, fragmentCode);
}

bool Shader::LoadFromCode(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    unsigned int vertex, fragment;

    auto* vertexShaderCodePtr = vertexShaderCode.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCodePtr, NULL);
    glCompileShader(vertex);
    if (GetSuccessStatus(vertex, GL_COMPILE_STATUS) == false) {
        return false;
    }

    auto* fragmentShaderCodePtr = fragmentShaderCode.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCodePtr, NULL);
    glCompileShader(fragment);
    if (GetSuccessStatus(fragment, GL_COMPILE_STATUS) == false) {
        return false;
    }

    shaderId = glCreateProgram();
    glAttachShader(shaderId, vertex);
    glAttachShader(shaderId, fragment);
    glLinkProgram(shaderId);
    if (GetSuccessStatus(shaderId, GL_LINK_STATUS) == false) {
        return false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

void Shader::SetUniform(const std::string &name, float value) const
{
    auto pos = glGetUniformLocation(shaderId, name.c_str());
    glUniform1f(pos, value);
}

void Shader::SetUniform(const std::string& name, const glm::mat3x3& value) const
{
    auto pos = glGetUniformLocation(shaderId, name.c_str());
    glUniformMatrix3fv(pos, 1, GL_FALSE, &value[0][0]);
}

bool Shader::GetSuccessStatus(int shader, GLenum name)
{
    int success;

    glGetShaderiv(shader, name, &success);
    if (success == false){
        char infoLog[1024];

        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        LOG_ERROR("shader error %s", infoLog);

        return false;
    }

    return true;
}

void Shader::Use() const
{
    glUseProgram(shaderId);
}
