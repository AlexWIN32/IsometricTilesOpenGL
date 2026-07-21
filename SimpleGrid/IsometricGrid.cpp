//based on https://pikuma.com/blog/isometric-projection-in-games

#include "IsometricGrid.h"
#include "consoleLog.h"
#include <vector>

static glm::vec2 gridToIsometric(const glm::vec2& coords, const glm::vec2& tileSize)
{
    float xStart = 0.0f;
    float yStart = 0.0f;

    return { xStart + (coords.x - coords.y) * tileSize.x * 0.5f, yStart + (coords.x + coords.y) * tileSize.y * 0.5f};

    /*
    we can also express this transform in matrix form

    xscreen = xStart + (x - y) * tileSize.x*0.5
    yscreen = yStart + (x + y) * tileSize.y*0.5

    x * tileSize.x*0.5 - y * tileSize.x*0.5 + xStart = xscreen
    x * tileSize.y*0.5 + y * tileSize.y*0.5 + yStart = yscreen

    x * tileSize.x*0.5 + y * -tileSize.x*0.5 + xStart = xscreen
    x * tileSize.y*0.5 + y * tileSize.y*0.5 + yStart = yscreen

              |tileSize.x*0.5   tileSize.y*0.5 0|
    |x y 1| * |-tileSize.x*0.5  tileSize.y*0.5 0| = |xscreen yscreen 1|
              |xStart           yStart         1|

    glm::mat3x3 transform(
        tileSize.x * 0.5f,  tileSize.y * 0.5f, 0.0f,
        -tileSize.x * 0.5f, tileSize.y * 0.5f, 0.0f,
        xStart,             yStart,            1.0f
    );

    return transform * glm::vec3(coords, 1.0f);
    */
}

static glm::vec2 isometricToGrid(const glm::vec2& coords, const glm::vec2& tileSize)
{
    /*
    https://chatgpt.com/share/6a5ea1cf-f7b8-83eb-be8a-d0b253f31a6f

    we have
    x_screen = x_start + (x - y) * TILE_WIDTH/2
    y_screen = y_start + (x + y) * TILE_HEIGHT/2

    where x_screen and y_screen is our screen coords, x_start and y_start is 
    grid offset. We need to solve this system for x and y

    lets define
    A = x_screen - x_start
    B = y_screen - y_start

    then

    A = (x - y) * TILE_WIDTH/2
    B = (x + y) * TILE_HEIGHT/2
    x - y = 2*A/TILE_WIDTH
    x + y = 2*B/TILE_HEIGHT

    add two equations to eliminate y

    2*x = 2*A/TILE_WIDTH + 2*B/TILE_HEIGHT
    x = A/TILE_WIDTH + B/TILE_HEIGHT

    subtract first equation from the second to eliminate x

    2*y = 2*B/TILE_HEIGHT - 2*A/TILE_WIDTH
    y = B/TILE_HEIGHT - A/TILE_WIDTH

    now we need to substitute A and B

    x = (x_screen - x_start)/TILE_WIDTH + (y_screen - y_start)/TILE_HEIGHT
    y = (y_screen - y_start)/TILE_HEIGHT - (x_screen - x_start)/TILE_WIDTH
    */

    float xStart = 0.0f;
    float yStart = 0.0f;

    return {
        (coords.x - xStart) / tileSize.x + (coords.y - yStart) / tileSize.y,
        (coords.y - yStart) / tileSize.y - (coords.x - xStart) / tileSize.x
    };

    /*
    like in gridToIsometric() we can express grid to isometric transform in matrix form
    and than multiply isometric coords by its inverse

    glm::mat3x3 transform(
        tileSize.x * 0.5f,  tileSize.y * 0.5f, 0.0f,
        -tileSize.x * 0.5f, tileSize.y * 0.5f, 0.0f,
        xStart,             yStart,            1.0f
    );

    return glm::inverse(transform) * glm::vec3(coords, 1.0f);
    */
}

void IsometricGrid::PickCell(const glm::vec2& cursorPos)
{
    const auto isometricCoords = isometricToGrid(cursorPos, tileSize);

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

void IsometricGrid::Init(const glm::vec2& gridSize, const glm::vec2& tileSize)
{
    this->gridSize = gridSize;
    this->tileSize = tileSize;

    fillBuffer.Init();
    lineBuffer.Init();

    UpdateVertices();
}

void IsometricGrid::UpdateVertices()
{
    std::vector<VertexBuffer::Vertex> fillVertices;
    std::vector<VertexBuffer::Vertex> lineVertices;

    const glm::vec3 htmlGreen = { 0.0f, 0.5f, 0.0f };
    const glm::vec3 yellow = { 1.0f, 1.0f, 0.0f };

    for (int c = 0; c < gridSize.x; ++c) {
        for (int r = 0; r < gridSize.y; ++r) {
            const auto cellCenter = gridToIsometric({ c, r }, tileSize);
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
