//based on https://pikuma.com/blog/isometric-projection-in-games

#include "IsometricGrid.h"
#include <Rendering/ColoredVertex.h>
#include <consoleLog.h>
#include <Convertions.h>
#include <vector>

void IsometricGrid::PickCell(const glm::vec2& cursorPos)
{
    const auto isometricCoords = IsometricToWorldGrid(cursorPos, tileSize, origin);

    pickedTile.x = round(isometricCoords.x);
    pickedTile.y = round(isometricCoords.y);

    LOG_INFO("tile under cursor %f %f", pickedTile.x, pickedTile.y);

    if (pickedTile.x >= 0.0f && pickedTile.x < gridSize.x &&
        pickedTile.y >= 0.0f && pickedTile.y < gridSize.y)
    {
        pickedTileInsideGrid = true;
    }
    else {
        pickedTileInsideGrid = false;
    }

    UpdateVertices();
}

void IsometricGrid::Init(const glm::vec2& gridSize, const glm::vec2& tileSize, const glm::vec2& origin)
{
    this->gridSize = gridSize;
    this->tileSize = tileSize;
    this->origin = origin;

    fillBuffer.Init();
    lineBuffer.Init();

    UpdateVertices();
}

void IsometricGrid::UpdateVertices()
{
    std::vector<ColoredVertex> fillVertices;
    std::vector<ColoredVertex> lineVertices;

    const glm::vec3 htmlGreen = { 0.0f, 0.5f, 0.0f };
    const glm::vec3 yellow = { 1.0f, 1.0f, 0.0f };

    for (int c = 0; c < gridSize.x; ++c) {
        for (int r = 0; r < gridSize.y; ++r) {
            const auto cellCenter = WorldGridToIsometric({ c, r }, tileSize, origin);
            const auto north = cellCenter + glm::vec2(0.0f, tileSize.y * 0.5f);
            const auto south = cellCenter + glm::vec2(0.0f, -tileSize.y * 0.5f);
            const auto west = cellCenter + glm::vec2(-tileSize.x * 0.5f, 0.0f);
            const auto east = cellCenter + glm::vec2(tileSize.x * 0.5f, 0.0f);

            const auto fillColor = pickedTileInsideGrid == true && c == static_cast<int>(pickedTile.x) && r == static_cast<int>(pickedTile.y) ? yellow : htmlGreen;

            fillVertices.push_back({ west, fillColor });
            fillVertices.push_back({ north, fillColor });
            fillVertices.push_back({ east, fillColor });
            fillVertices.push_back({ east, fillColor });
            fillVertices.push_back({ south, fillColor });
            fillVertices.push_back({ west, fillColor });

            lineVertices.push_back({ west, yellow });
            lineVertices.push_back({ north, yellow });
            lineVertices.push_back({ north, yellow });
            lineVertices.push_back({ east, yellow });
            lineVertices.push_back({ east, yellow });
            lineVertices.push_back({ south, yellow });
            lineVertices.push_back({ south, yellow });
            lineVertices.push_back({ west, yellow });
        }
    }

    fillBuffer.Update(fillVertices);
    lineBuffer.Update(lineVertices);
}

void IsometricGrid::Draw()
{
    fillBuffer.Draw(GL_TRIANGLES, gridSize.x * gridSize.y * 6);
    lineBuffer.Draw(GL_LINES, gridSize.x * gridSize.y * 8);
}
