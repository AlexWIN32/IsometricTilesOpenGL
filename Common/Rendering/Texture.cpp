#include "texture.h"
#include "stb_image.h"
#include <consoleLog.h>

Texture::~Texture()
{
    glDeleteTextures(1, &textureId);
}

bool Texture::Load(const std::string& path, float pixelsPerUnit)
{
    this->pixelsPerUnit = pixelsPerUnit;

    int channels;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data == nullptr) {
        LOG_ERROR("cant load texture %s", path.c_str());
        return false;
    }

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::Use(GLuint uniformIndex, GLuint textureIndex) const
{
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(uniformIndex, textureIndex);
}

void Texture::Unuse(GLuint textureIndex) const
{
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

int Texture::GetWidth() const
{
    return width;
}

int Texture::GetHeight() const
{
    return height;
}

float Texture::GetWidthInUints() const
{
    return static_cast<float>(width) / pixelsPerUnit;
}

float Texture::GetHeightInUints() const
{
    return static_cast<float>(height) / pixelsPerUnit;
}

float Texture::GetPixelsPerUint() const
{
    return pixelsPerUnit;
}
