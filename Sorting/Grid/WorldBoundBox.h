#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <Rendering/LineVertex.h>
#include <vector>

class WorldBoundBox
{
public:
    const glm::vec3& GetMinCoord() const;
    const glm::vec3& GetMaxCoord() const;
    glm::vec2 GetIsometricSpaceCenter(const glm::vec2& gridStart, const glm::vec2& tileSize) const;
    std::vector<glm::vec2> GetIsometricSpaceCorners(const glm::vec2& gridStart, const glm::vec2& tileSize) const;
    void AddToDebugVertices(const glm::vec3& color, std::vector<LineVertex>& vertices, const glm::vec2& gridStart, const glm::vec2& tileSize) const;
    static WorldBoundBox CalculateForObject(const glm::vec3& center, const glm::vec3& size);
private:
    glm::vec3 minCoord;
    glm::vec3 maxCoord;
};

