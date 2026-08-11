#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <Rendering/VertexAttribute.h>

struct LineVertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static int GetVertexSize()
    {
        return 5 * sizeof(float);
    }
    static std::vector<VertexAttribute> GetVertexAttributes()
    {
        return {
            {2, 0}, //pos
            {3, 2 * sizeof(float)}, //color
        };
    }
};

