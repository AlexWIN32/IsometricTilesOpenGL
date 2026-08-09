#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture() = default;
    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;
    ~Texture();

    bool Load(const std::string& path, float pixelsPerUnit);
    void Use(GLuint uniformIndex, GLuint textureIndex) const;
    void Unuse(GLuint textureIndex) const;
    int GetWidth() const;
    int GetHeight() const;
    float GetWidthInUints() const;
    float GetHeightInUints() const;
    float GetPixelsPerUint() const;
private:
    float pixelsPerUnit;
    GLuint textureId = 0;
    int width = 0;
    int height = 0;
};
