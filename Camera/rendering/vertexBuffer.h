#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include <glad/glad.h>

class VertexBuffer
{
public:
    struct Vertex
    {
        glm::vec2 position;
        glm::vec3 color;
    };

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer() = default;
    VertexBuffer(VertexBuffer&&) = default;
    VertexBuffer& operator=(VertexBuffer&&) = default;
    ~VertexBuffer();
    void Init();
    void Update(const std::vector<Vertex>& vertices) const;
    void Draw(GLenum mode, GLsizei count) const;
private:
    GLuint vbo = 0;
    GLuint vao = 0;
};
