#include "WorldBoundBox.h"
#include "Convertions.h"

static void addLine(std::vector<ColoredVertex>& vertices, const std::vector<glm::vec2>& coords, int ind1, int ind2, const glm::vec3& color)
{
    vertices.push_back({coords[ind1], color});
    vertices.push_back({coords[ind2], color});
}

const glm::vec3& WorldBoundBox::GetMinCoord() const
{
    return minCoord;
}

const glm::vec3& WorldBoundBox::GetMaxCoord() const
{
    return maxCoord;
}

glm::vec2 WorldBoundBox::GetIsometricSpaceCenter(const glm::vec2& gridStart, const glm::vec2& tileSize) const
{
    const auto center = (minCoord + maxCoord) * 0.5f;

    auto res = WorldGridToIsometric(center, tileSize, gridStart);

    res.y -= minCoord.z;

    return res;
}

std::vector<glm::vec2> WorldBoundBox::GetIsometricSpaceCorners(const glm::vec2& gridStart, const glm::vec2& tileSize) const
{
    std::vector<glm::vec3> boxVerticesW =
    {
        {minCoord.x, minCoord.y, minCoord.z},
        {minCoord.x, maxCoord.y, minCoord.z},
        {maxCoord.x, maxCoord.y, minCoord.z},
        {maxCoord.x, minCoord.y, minCoord.z},
        {minCoord.x, minCoord.y, maxCoord.z},
        {minCoord.x, maxCoord.y, maxCoord.z},
        {maxCoord.x, maxCoord.y, maxCoord.z},
        {maxCoord.x, minCoord.y, maxCoord.z},
    };

    std::vector<glm::vec2> boxVerticesI;

    for (const auto& vertexW : boxVerticesW) {
        auto vertexS = WorldGridToIsometric(vertexW, tileSize, gridStart);

        vertexS.y -= vertexW.z;

        boxVerticesI.push_back(vertexS);
    }

    return boxVerticesI;
}

void WorldBoundBox::AddToDebugVertices(const glm::vec3& color, std::vector<ColoredVertex>& vertices, const glm::vec2& gridStart, const glm::vec2& tileSize) const
{
    const auto boxVerticesI = GetIsometricSpaceCorners(gridStart, tileSize);

    addLine(vertices, boxVerticesI, 0, 1, color);
    addLine(vertices, boxVerticesI, 1, 2, color);
    addLine(vertices, boxVerticesI, 2, 3, color);
    addLine(vertices, boxVerticesI, 3, 0, color);

    addLine(vertices, boxVerticesI, 4, 5, color);
    addLine(vertices, boxVerticesI, 5, 6, color);
    addLine(vertices, boxVerticesI, 6, 7, color);
    addLine(vertices, boxVerticesI, 7, 4, color);

    addLine(vertices, boxVerticesI, 0, 4, color);
    addLine(vertices, boxVerticesI, 1, 5, color);
    addLine(vertices, boxVerticesI, 2, 6, color);
    addLine(vertices, boxVerticesI, 3, 7, color);
}

/*
WorldBoundBox WorldBoundBox::CalculateForTile(const glm::vec3& bottomLeftCoord)
{
    //since tile occupies one unit cell and has no height it's size is (1,1,0)

    WorldBoundBox res;

    res.minCoord = bottomLeftCoord;
    res.maxCoord = bottomLeftCoord + glm::vec3(1.0f, 1.0f, 0.0f);

    return res;
}
*/

WorldBoundBox WorldBoundBox::CalculateForObject(const glm::vec3& center, const glm::vec3& size)
{
    const auto halfSize = size * 0.5f;

    WorldBoundBox res;

    res.minCoord = center - halfSize;
    res.maxCoord = center + halfSize;

    res.minCoord.z = center.z;
    res.maxCoord.z = center.z + size.z;

    return res;
}

