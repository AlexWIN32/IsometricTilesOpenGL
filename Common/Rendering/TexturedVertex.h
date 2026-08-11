#pragma once
#include <glm/vec2.hpp>
#include <vector>
#include <Rendering/VertexAttribute.h>

struct TexturedVertex
{
    glm::vec2 pos;
    glm::vec2 texCoord;

    static int GetVertexSize()
    {
        return 4 * sizeof(float);
    }
    static std::vector<VertexAttribute> GetVertexAttributes()
    {
        return {
            {2, 0}, //pos
            {2, 2 * sizeof(float)}, //texcoord
        };
    }
};
