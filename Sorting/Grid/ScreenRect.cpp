#include <Grid/ScreenRect.h>
#include <Grid/WorldBoundBox.h>
#include <vector>

static void addLine(std::vector<LineVertex>& vertices, const std::vector<glm::vec2>& coords, int ind1, int ind2, const glm::vec3& color)
{
    vertices.push_back({coords[ind1], color});
    vertices.push_back({coords[ind2], color});
}

bool ScreenRect::Overlaps(const ScreenRect& other) const
{
    return maxPos.x >= other.minPos.x && minPos.x <= other.maxPos.x
           && maxPos.y >= other.minPos.y && minPos.y <= other.maxPos.y;
}

void ScreenRect::AddToDebugVertices(const glm::vec3& color, std::vector<LineVertex>& vertices) const
{
    std::vector<glm::vec2> corners =
    {
        {minPos.x, minPos.y},
        {minPos.x, maxPos.y},
        {maxPos.x, maxPos.y},
        {maxPos.x, minPos.y},
    };

    addLine(vertices, corners, 0, 1, color);
    addLine(vertices, corners, 1, 2, color);
    addLine(vertices, corners, 2, 3, color);
    addLine(vertices, corners, 3, 0, color);
}

ScreenRect ScreenRect::FromWorldBoundBox(WorldBoundBox& worldBoundBox, const glm::vec2& gridStart, const glm::vec2& tileSize)
{
    ScreenRect res;
    res.maxPos.x = std::numeric_limits<float>::min();
    res.maxPos.y = std::numeric_limits<float>::min();

    res.minPos.x = std::numeric_limits<float>::max();
    res.minPos.y = std::numeric_limits<float>::max();

    const auto corners = worldBoundBox.GetIsometricSpaceCorners(gridStart, tileSize);

    for (const auto& corner : corners) {
        res.maxPos.x = std::max(res.maxPos.x, corner.x);
        res.maxPos.y = std::max(res.maxPos.y, corner.y);

        res.minPos.x = std::min(res.minPos.x, corner.x);
        res.minPos.y = std::min(res.minPos.y, corner.y);
    }

    return res;
}
