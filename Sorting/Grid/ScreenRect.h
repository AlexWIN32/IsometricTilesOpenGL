#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <Rendering/ColoredVertex.h>

class WorldBoundBox;

class ScreenRect
{
public:
    bool Overlaps(const ScreenRect& other) const;
    void AddToDebugVertices(const glm::vec3& color, std::vector<ColoredVertex>& vertices) const;
    static ScreenRect FromWorldBoundBox(WorldBoundBox& worldBoundBox, const glm::vec2& gridStart, const glm::vec2& tileSize);
private:
    glm::vec2 minPos;
    glm::vec2 maxPos;
};
