#pragma once

#include <glm/vec2.hpp>
#include <vector>

#include <glad/glad.h>

template<class TVertex>
class VertexBuffer
{
public:

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer() = default;
    VertexBuffer(VertexBuffer&& other)
    {
        this->vao = other.vao;
        this->vbo = other.vbo;

        other.vao = 0;
        other.vbo = 0;

    }
    VertexBuffer& operator=(VertexBuffer&& other)
    {
        this->vao = other.vao;
        this->vbo = other.vbo;

        other.vao = 0;
        other.vbo = 0;

        return *this;
    }
    ~VertexBuffer()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void Init()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        int attrInd = 0;

        const auto vertexSize = TVertex::GetVertexSize();
        for (const auto& attribute : TVertex::GetVertexAttributes()) {
            glVertexAttribPointer(attrInd, attribute.size, GL_FLOAT, GL_FALSE, vertexSize, (void*)attribute.offset);
            glEnableVertexAttribArray(attrInd);
            ++attrInd;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Update(const std::vector<TVertex>& vertices) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), vertices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Draw(GLenum mode, GLsizei count) const
    {
        glBindVertexArray(vao);
        glDrawArrays(mode, 0, count);
    }
private:
    GLuint vbo = 0;
    GLuint vao = 0;
};

