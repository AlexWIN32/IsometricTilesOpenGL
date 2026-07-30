#pragma once
#include <string>
#include <glad/glad.h>

class Shader
{
public:
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader() = default;
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;
    ~Shader() = default;

    bool LoadFromCode(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
    bool LoadFromFiles(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
    void Use() const;
    void SetUniform(const std::string& name, float value) const;
    GLuint GetUniformLocation(const std::string& name) const;
private:
    bool GetSuccessStatus(int shader, GLenum name);
    GLuint shaderId;
};
