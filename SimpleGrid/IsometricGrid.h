#pragma once
#include "rendering/vertexBuffer.h"
#include <glm/vec2.hpp>

class IsometricGrid
{
public:
    void Init(const glm::vec2& gridSize, const glm::vec2& tileSize);
    void PickCell(const glm::vec2& cursorPos);
    void Draw();
private:
    void UpdateVertices();
    bool pickedTileInsideGrid = false;
    glm::vec2 pickedTile;
    glm::vec2 gridSize;
    glm::vec2 tileSize;
    VertexBuffer fillBuffer;
    VertexBuffer lineBuffer;
};
